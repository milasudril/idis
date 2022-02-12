#ifndef IDIS_GPURES_UTILS_HPP
#define IDIS_GPURES_UTILS_HPP

#include "./swapchain.hpp"
#include "./image_view.hpp"
#include "./framebuffer.hpp"
#include "./command_pool.hpp"
#include "./pipeline.hpp"
#include "./semaphore.hpp"

#include "engine/error_handler/exception.hpp"

#include <vector>
#include <algorithm>

namespace idis::gpu_res
{
	inline std::vector<image_view> create_image_views_from(swapchain const& src)
	{
		std::vector<image_view> ret;
		std::ranges::transform(src.get_images(),
		                       std::back_inserter(ret),
		                       [dev = src.device(), format = src.image_format()](auto item) {
			                       return image_view{dev, item, format};
		                       });
		return ret;
	}

	inline std::vector<framebuffer> create_framebuffers_from(idis::vk_init::device& device,
	                                                         render_pass& rp,
	                                                         VkExtent2D dim,
	                                                         std::span<image_view> img_views)
	{
		std::vector<framebuffer> ret;
		std::ranges::transform(img_views,
		                       std::back_inserter(ret),
		                       [dev = device.handle(), rp_handle = rp.handle(), dim](auto& item) {
			                       return framebuffer{dev, rp_handle, dim, item.handle()};
		                       });

		return ret;
	}

	class command_recording_session
	{
	public:
		explicit command_recording_session(VkCommandBuffer handle): m_handle{handle}
		{
			VkCommandBufferBeginInfo begin_info{};
			begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			if(!vkBeginCommandBuffer(m_handle, &begin_info) != VK_SUCCESS)
			{
				exception{"lock command buffer", ""};
			}
		}

		~command_recording_session() { vkEndCommandBuffer(m_handle); }

		command_recording_session(command_recording_session const&) = delete;
		command_recording_session& operator=(command_recording_session const&) = delete;
		command_recording_session(command_recording_session&&)                 = delete;
		command_recording_session& operator=(command_recording_session&&) = delete;

		VkCommandBuffer command_buffer() { return m_handle; }

	private:
		VkCommandBuffer m_handle;
	};

	class render_pass_section
	{
	public:
		explicit render_pass_section(command_recording_session& session,
		                             VkFramebuffer fb,
		                             VkRenderPass rp,
		                             VkExtent2D extent)
		    : m_handle{session.command_buffer()}
		{
			VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

			VkRenderPassBeginInfo render_pass_info{};
			render_pass_info.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			render_pass_info.framebuffer       = fb;
			render_pass_info.renderPass        = rp;
			render_pass_info.renderArea.extent = extent;
			render_pass_info.clearValueCount   = 1;
			render_pass_info.pClearValues      = &clear_color;
			vkCmdBeginRenderPass(m_handle, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
		}

		~render_pass_section() { vkCmdEndRenderPass(m_handle); }

		render_pass_section(render_pass_section const&) = delete;
		render_pass_section& operator=(render_pass_section const&) = delete;
		render_pass_section(render_pass_section&&)                 = delete;
		render_pass_section& operator=(render_pass_section&&) = delete;

		render_pass_section& bind(VkCommandBuffer buffer, VkPipeline pipeline)
		{
			vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
			return *this;
		}

		// FIXME: Strongly typed integers here
		render_pass_section& draw(VkCommandBuffer buffer, int a, int b, int c, int d)
		{
			vkCmdDraw(buffer, a, b, c, d);
			return *this;
		}

	private:
		VkCommandBuffer m_handle;
	};

	inline auto acquire_next_image(vk_init::device& dev, swapchain& swp, semaphore& sem)
	{
		uint32_t ret{};
		vkAcquireNextImageKHR(
		    dev.handle(), swp.handle(), UINT64_MAX, sem.handle(), VK_NULL_HANDLE, &ret);
		return ret;
	}

	inline auto submit(VkQueue queue,
	                   VkCommandBuffer cmd_buffer,
	                   semaphore& wait_for,
	                   semaphore& signal)
	{
		VkSubmitInfo submit_info{};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		std::array<VkSemaphore, 1> const wait_sem{wait_for.handle()};
		submit_info.waitSemaphoreCount = static_cast<uint32_t>(std::size(wait_sem));
		submit_info.pWaitSemaphores    = std::data(wait_sem);

		std::array<VkPipelineStageFlags, 1> const wait_stages{
		    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submit_info.pWaitDstStageMask  = std::data(wait_stages);
		submit_info.commandBufferCount = 1;

		submit_info.pCommandBuffers = &cmd_buffer;

		std::array<VkSemaphore, 1> const signal_sem{signal.handle()};
		submit_info.signalSemaphoreCount = static_cast<uint32_t>(std::size(signal_sem));
		submit_info.pSignalSemaphores    = std::data(signal_sem);

		if(vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS)
		{
			throw exception{"submit command buffer", ""};
		}
	}

	inline auto present(VkQueue queue, swapchain& swp, uint32_t image_index, semaphore& wait_for)
	{
		VkPresentInfoKHR present_info{};
		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		std::array<VkSemaphore, 1> const wait_sem{wait_for.handle()};
		present_info.waitSemaphoreCount = static_cast<uint32_t>(std::size(wait_sem));
		present_info.pWaitSemaphores    = std::data(wait_sem);

		std::array<VkSwapchainKHR, 1> swapchains{swp.handle()};
		present_info.swapchainCount = static_cast<uint32_t>(std::size(swapchains));
		present_info.pSwapchains    = std::data(swapchains);
		present_info.pImageIndices  = &image_index;

		vkQueuePresentKHR(queue, &present_info);
	}
}

#endif
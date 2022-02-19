#ifndef IDIS_GPURES_UTILS_HPP
#define IDIS_GPURES_UTILS_HPP

#include "./swapchain.hpp"
#include "./image_view.hpp"
#include "./framebuffer.hpp"
#include "./command_pool.hpp"
#include "./pipeline.hpp"
#include "./semaphore.hpp"
#include "./fence.hpp"
#include "./buffer.hpp"

#include "engine/error_handling/exception.hpp"
#include "engine/vk_init/error.hpp"

#include <vector>
#include <algorithm>

namespace idis::gpu_res
{
	inline auto create_image_views_from(std::reference_wrapper<swapchain const> src)
	{
		std::vector<image_view> ret{};
		std::ranges::transform(
		    get_images(src),
		    std::back_inserter(ret),
		    [dev = src.get().device(), format = src.get().image_format()](auto item) {
			    return image_view{dev, item, format};
		    });
		return ret;
	}

	inline auto create_framebuffers_from(render_pass const& matching_rp,
	                                     VkExtent2D dim,
	                                     std::span<image_view const> img_views)
	{
		std::vector<framebuffer> ret{};
		std::ranges::transform(
		    img_views,
		    std::back_inserter(ret),
		    [dev = matching_rp.device(), rp_handle = matching_rp.handle(), dim](auto& item) {
			    return framebuffer{dev, rp_handle, dim, item.handle()};
		    });

		return ret;
	}

	class command_recording_session
	{
	public:
		explicit command_recording_session(VkCommandBuffer handle): m_handle{handle}
		{
			vkResetCommandBuffer(handle, 0);
			VkCommandBufferBeginInfo begin_info{};
			begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			if(auto res = vkBeginCommandBuffer(m_handle, &begin_info); res != VK_SUCCESS)
			{
				exception{"record commands", to_string(vk_init::error{res})};
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

		// TODO: Type-checked binding
		template<auto BufferUsage, auto AllocationFlags>
		requires(BufferUsage == VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) render_pass_section& bind(
		    VkCommandBuffer cmdbuff,
		    std::reference_wrapper<buffer<BufferUsage, AllocationFlags> const> buff,
		    uint32_t binding = 0)
		{
			VkDeviceSize offsets{};
			auto handle = buff.get().handle();
			vkCmdBindVertexBuffers(cmdbuff, binding, 1u, &handle, &offsets);
			return *this;
		}

		// FIXME: Strongly typed integers here
		// TODO: use capacity of current buffer(s)
		render_pass_section& draw(VkCommandBuffer buffer, int a, int b, int c, int d)
		{
			vkCmdDraw(buffer, a, b, c, d);
			return *this;
		}

	private:
		VkCommandBuffer m_handle;
	};

	inline void wait_and_reset(fence const& fence)
	{
		auto fence_handle = fence.handle();
		vkWaitForFences(fence.device(), 1, &fence_handle, VK_TRUE, UINT64_MAX);
		vkResetFences(fence.device(), 1, &fence_handle);
	}

	template<class OutOfDateHandler>
	auto acquire_next_image(vk_init::device const& dev,
	                        swapchain const& swp,
	                        signal_semaphore sem,
	                        OutOfDateHandler&& on_out_of_date)
	{
		while(true)
		{
			uint32_t ret{};
			auto const res = vkAcquireNextImageKHR(
			    dev.handle(), swp.handle(), UINT64_MAX, sem.handle(), VK_NULL_HANDLE, &ret);
			switch(res)
			{
				case VK_SUCCESS: return ret;
				case VK_SUBOPTIMAL_KHR: return ret;
				case VK_ERROR_OUT_OF_DATE_KHR: on_out_of_date(); break;
				default: throw exception{"acquire next image", to_string(vk_init::error{res})};
			}
		}
	}

	inline auto submit(VkQueue queue,
	                   VkCommandBuffer cmd_buffer,
	                   wait_semaphore wait_for,
	                   signal_semaphore signal,
	                   signal_fence signal_fence)
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

		if(auto res = vkQueueSubmit(queue, 1, &submit_info, signal_fence.handle());
		   res != VK_SUCCESS)
		{
			throw exception{"submit command buffer", to_string(vk_init::error{res})};
		}
	}

	inline auto present(VkQueue queue,
	                    swapchain& swp,
	                    uint32_t image_index,
	                    wait_semaphore wait_for)
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
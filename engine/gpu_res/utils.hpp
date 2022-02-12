#ifndef IDIS_GPURES_UTILS_HPP
#define IDIS_GPURES_UTILS_HPP

#include "./swapchain.hpp"
#include "./image_view.hpp"
#include "./framebuffer.hpp"
#include "./command_pool.hpp"

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

	private:
		VkCommandBuffer m_handle;
	};
}

#endif
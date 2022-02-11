//@	{
//@	 "target":{"name":"framebuffer.o"}
//@	}

#include "./framebuffer.hpp"

namespace
{
	auto create_framebuffer(VkDevice device, VkRenderPass rp, idis::gpu_res::image_view& img_view)
	{
		VkFramebufferCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		create_info.renderPass = rp;
		create_info.attachmentCount = 1;
		auto img_view_handle = img_view.handle();
		create_info.pAttachments = &img_view_handle;
//		create_info.width = swapChainExtent.width;
//		create_info.height = swapChainExtent.height;
		create_info.layers = 1;

		VkFramebuffer framebuffer{};
		if(vkCreateFramebuffer(device, &create_info, nullptr, &framebuffer) != VK_SUCCESS)
		{
			throw idis::exception{"create framebuffer", ""};
		}

		return idis::gpu_res::framebuffer::handle_type{
		                      framebuffer, idis::gpu_res::framebuffer_deleter{device}};
	}
}

idis::gpu_res::framebuffer::framebuffer(VkDevice device, VkRenderPass rp, image_view& img_view)
    : m_handle{create_framebuffer(device, rp, img_view)}
{
}
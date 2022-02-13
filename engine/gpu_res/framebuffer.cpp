//@	{
//@	 "target":{"name":"framebuffer.o"}
//@	}

#include "./framebuffer.hpp"

#include "engine/vk_init/error.hpp"

namespace
{
	auto create_framebuffer(VkDevice device, VkRenderPass matching_rp, VkExtent2D dim, VkImageView img_view)
	{
		VkFramebufferCreateInfo create_info{};
		create_info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		create_info.renderPass      = matching_rp;
		create_info.attachmentCount = 1;
		create_info.pAttachments    = &img_view;
		create_info.width           = dim.width;
		create_info.height          = dim.height;
		create_info.layers          = 1;

		VkFramebuffer framebuffer{};
		if(auto res = vkCreateFramebuffer(device, &create_info, nullptr, &framebuffer); res != VK_SUCCESS)
		{
			throw idis::exception{"create framebuffer", to_string(idis::vk_init::error{res})};
		}

		return idis::gpu_res::framebuffer::handle_type{framebuffer,
		                                               idis::gpu_res::framebuffer_deleter{device}};
	}
}

idis::gpu_res::framebuffer::framebuffer(VkDevice device,
                                        VkRenderPass matching_rp,
                                        VkExtent2D dim,
                                        VkImageView img_view)
    : m_handle{create_framebuffer(device, matching_rp, dim, img_view)}
{
}
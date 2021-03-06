//@	{"target":{"name":"image_view.o"}}

#include "./image_view.hpp"

#include "engine/vk_init/error.hpp"

namespace
{
	auto create_image_view(VkDevice device,
	                       VkImage image,
	                       idis::gpu_res::image_descriptor const& descriptor)
	{
		VkImageViewCreateInfo create_info{};
		create_info.sType        = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		create_info.image        = image;
		create_info.viewType     = VK_IMAGE_VIEW_TYPE_2D;
		create_info.format       = descriptor.format;
		create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.subresourceRange.aspectMask =
		    (descriptor.usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
		        ? VK_IMAGE_ASPECT_DEPTH_BIT
		        : VK_IMAGE_ASPECT_COLOR_BIT;
		create_info.subresourceRange.baseMipLevel   = 0;
		create_info.subresourceRange.levelCount     = 1;
		create_info.subresourceRange.baseArrayLayer = 0;
		create_info.subresourceRange.layerCount     = 1;

		VkImageView img_view{};
		if(auto res = vkCreateImageView(device, &create_info, nullptr, &img_view);
		   res != VK_SUCCESS)
		{
			throw idis::exception{"create image view", to_string(idis::vk_init::error{res})};
		}

		return idis::gpu_res::image_view::handle_type{img_view,
		                                              idis::gpu_res::image_view_deleter{device}};
	}

	auto create_image_view(VkDevice device, VkImage image, VkFormat format)
	{
		idis::gpu_res::image_descriptor descriptor;
		descriptor.format = format;
		descriptor.usage  = static_cast<VkImageUsageFlagBits>(0);
		return create_image_view(device, image, descriptor);
	}
}


idis::gpu_res::image_view::image_view(VkDevice device, VkImage image, VkFormat format)
    : m_handle{create_image_view(device, image, format)}
{
}

idis::gpu_res::image_view::image_view(VkDevice device,
                                      VkImage image,
                                      image_descriptor const& descriptor)
    : m_handle{create_image_view(device, image, descriptor)}
{
}
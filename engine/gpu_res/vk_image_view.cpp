//@	{"target":{"name":"vk_image_view.o"}}

#include "./vk_image_view.hpp"

namespace
{
	auto create_vk_image_view(VkDevice device, VkImage image, VkFormat format)
	{
		VkImageViewCreateInfo create_info{};
		create_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		create_info.image                           = image;
		create_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
		create_info.format                          = format;
		create_info.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
		create_info.subresourceRange.baseMipLevel   = 0;
		create_info.subresourceRange.levelCount     = 1;
		create_info.subresourceRange.baseArrayLayer = 0;
		create_info.subresourceRange.layerCount     = 1;

		VkImageView img_view{};
		if(vkCreateImageView(device, &create_info, nullptr, &img_view) != VK_SUCCESS)
		{
			throw idis::exception{"create vulkan image view", ""};
		}

		return idis::gpu_res::vk_image_view::handle_type{
		    img_view, idis::gpu_res::vk_image_view_deleter{device}};
	}
}


idis::gpu_res::vk_image_view::vk_image_view(VkDevice device, VkImage image, VkFormat format)
    : m_handle{create_vk_image_view(device, image, format)}
{
}

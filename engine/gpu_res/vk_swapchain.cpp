//@	{
//@	 "target":{"name":"vk_swapchain.o"}
//@	}

#include "./vk_swapchain.hpp"

namespace
{
	auto create_vk_swapchain(idis::wm::vk_device& device,
	                         idis::wm::vk_surface& surface,
	                         uint32_t image_count,
	                         VkSurfaceFormatKHR const& surface_format,
	                         VkExtent2D const& image_extent,
	                         VkPresentModeKHR present_mode,
	                         VkSurfaceTransformFlagBitsKHR transform)
	{
		VkSwapchainCreateInfoKHR create_info{};
		create_info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		create_info.surface          = surface.handle();
		create_info.minImageCount    = image_count;
		create_info.imageFormat      = surface_format.format;
		create_info.imageColorSpace  = surface_format.colorSpace;
		create_info.imageExtent      = image_extent;
		create_info.imageArrayLayers = 1;
		create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		std::array<uint32_t, 2> queue_families{
		    static_cast<uint32_t>(device.get_graphics_queue_family()),
		    static_cast<uint32_t>(device.get_surface_queue_family())};

		if(queue_families[0] == queue_families[1])
		{
			create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}
		else
		{
			create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
			create_info.queueFamilyIndexCount = std::size(queue_families);
			create_info.pQueueFamilyIndices   = std::data(queue_families);
		}

		create_info.preTransform   = transform;
		create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		create_info.presentMode    = present_mode;
		create_info.clipped        = VK_TRUE;
		create_info.oldSwapchain   = VK_NULL_HANDLE;

		VkSwapchainKHR swapchain{};
		if(vkCreateSwapchainKHR(device.handle(), &create_info, nullptr, &swapchain) != VK_SUCCESS)
		{
			throw idis::exception{"create vulkan swapchain", ""};
		}

		return idis::wm::vk_swapchain::handle_type{
		    swapchain, idis::wm::detail::vk_swapchain_deleter{device.handle()}};
	}
}

idis::wm::vk_swapchain::vk_swapchain(vk_device& device,
                                     vk_surface& surface,
                                     uint32_t image_count,
                                     VkSurfaceFormatKHR const& surface_format,
                                     VkExtent2D const& image_extent,
                                     VkPresentModeKHR present_mode,
                                     VkSurfaceTransformFlagBitsKHR transform)
    : m_handle{create_vk_swapchain(
        device, surface, image_count, surface_format, image_extent, present_mode, transform)}
{
}
//@	{
//@	 "target":{"name":"swapchain.o"}
//@	}

#include "./swapchain.hpp"

#include "engine/vk_init/error.hpp"

namespace
{
	auto create_swapchain(std::reference_wrapper<idis::vk_init::device const> device,
	                      std::reference_wrapper<idis::vk_init::surface const> surface)
	{
		auto const physical_device = device.get().device_info().device;
		auto const surface_caps    = get_surface_capabilities(physical_device, surface);
		auto const image_count     = idis::vk_init::get_image_count(surface_caps);
		auto const surface_format  = select_surface_format(physical_device, surface);
		auto const image_extent    = select_extent(surface_caps, surface.get().target_window());
		auto const present_mode    = select_present_mode(physical_device, surface);
		auto const transform       = surface_caps.currentTransform;

		VkSwapchainCreateInfoKHR create_info{};
		create_info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		create_info.surface          = surface.get().handle();
		create_info.minImageCount    = image_count;
		create_info.imageFormat      = surface_format.format;
		create_info.imageColorSpace  = surface_format.colorSpace;
		create_info.imageExtent      = image_extent;
		create_info.imageArrayLayers = 1;
		create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		std::array<uint32_t, 2> queue_families{
		    static_cast<uint32_t>(device.get().graphics_queue_family()),
		    static_cast<uint32_t>(device.get().surface_queue_family())};

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
		if(auto res =
		       vkCreateSwapchainKHR(device.get().handle(), &create_info, nullptr, &swapchain);
		   res != VK_SUCCESS)
		{
			throw idis::exception{"create vulkan swapchain", to_string(idis::vk_init::error{res})};
		}

		return std::tuple{idis::gpu_res::swapchain::handle_type{
		                      swapchain, idis::gpu_res::swapchain_deleter{device.get().handle()}},
		                  surface_format.format,
		                  image_extent};
	}
}

idis::gpu_res::swapchain::swapchain(std::reference_wrapper<vk_init::device const> device,
                                    std::reference_wrapper<vk_init::surface const> surface)
    : m_data{create_swapchain(device, surface)}
{
}
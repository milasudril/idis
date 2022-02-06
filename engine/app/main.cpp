//@	{"target":{"name":"main.o"}}

#include "./main.hpp"

#include "engine/error_handler/crash_handler.hpp"
#include "engine/vk_init/instance.hpp"
#include "engine/vk_init/surface.hpp"
#include "engine/vk_init/device.hpp"
#include "engine/window_manager/window.hpp"
#include "engine/gpu_res/vk_swapchain.hpp"

#include <algorithm>
#include <cstdio>

#include <cassert>

namespace
{
	size_t rank(VkPresentModeKHR value)
	{
		switch(value)
		{
			case VK_PRESENT_MODE_MAILBOX_KHR: return 0;
			case VK_PRESENT_MODE_IMMEDIATE_KHR: return 1;
			default: return static_cast<size_t>(-1);
		}
	}
}

int idis::app::main(int, char**)
{
	crash_handler::arm();

	printf("# Initiating vulkan\n\n");

	// Create vulkan instance

	vk_init::instance eyafjallajökull;

	// Create surface

	wm::window_base window{1024, 640, "Idis"};
	vk_init::surface surface{eyafjallajökull, window};

	auto device_info = select_device("", eyafjallajökull.system_info(), surface);

	// create device

	vk_init::device device{device_info};

	auto graphics_queue = device.get_graphics_queue();
	assert(graphics_queue != nullptr);

	auto surface_queue = device.get_surface_queue();
	assert(surface_queue != nullptr);

	// create swapchain
	auto const surface_formats = get_surface_formats(device_info.device, surface);

	auto const surface_format =
	    std::ranges::find_if(surface_formats,
	                         [](auto const& item)
	                         {
		                         return item.format == VK_FORMAT_B8G8R8A8_SRGB
		                                && item.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	                         });

	if(surface_format == std::end(surface_formats))
	{
		throw exception{"configure surface", "No suitable surface format found"};
	}

	auto present_modes = get_surface_present_modes(device_info.device, surface);
	std::ranges::sort(present_modes, [](auto a, auto b) { return rank(a) < rank(b); });

	auto const present_mode = present_modes[0];
	if(!(present_mode == VK_PRESENT_MODE_MAILBOX_KHR
	     || present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR))
	{
		throw exception{"configure surface", "No suitable present mode found"};
	}

	printf("## Selected present mode %d\n\n", present_mode);

	auto surface_caps = get_surface_capabilities(device_info.device, surface);

	gpu_res::vk_swapchain swapchain{device,
	                                surface,
	                                vk_init::get_image_count(surface_caps),
	                                *surface_format,
	                                select_extent(surface_caps, window),
	                                present_mode,
	                                surface_caps.currentTransform};

	auto const images = swapchain.get_images();
	assert(std::size(images) == vk_init::get_image_count(surface_caps));
	return 0;
}
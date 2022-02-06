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

int idis::app::main(int, char**)
{
	crash_handler::arm();

	vk_init::instance eyafjallajökull;
	wm::window_base window{1024, 640, "Idis"};
	vk_init::surface surface{eyafjallajökull, window};
	auto device_info = select_device("", eyafjallajökull.system_info(), surface);
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

	auto surface_caps = get_surface_capabilities(device_info.device, surface);

	gpu_res::vk_swapchain swapchain{device,
	                                surface,
	                                vk_init::get_image_count(surface_caps),
	                                *surface_format,
	                                select_extent(surface_caps, window),
	                                select_present_mode(device_info.device, surface),
	                                surface_caps.currentTransform};

	auto const images = swapchain.get_images();
	assert(std::size(images) == vk_init::get_image_count(surface_caps));
	return 0;
}
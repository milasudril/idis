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
	size_t rank(VkPhysicalDeviceType value)
	{
		switch(value)
		{
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return 0;
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return 1;
			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: return 1;
			case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			default: return static_cast<size_t>(-1);
		}
	}

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

	printf("# Initiating vulkan\n");
	vk_init::instance eyafjallajökull;
	printf("\n");

	auto& system_info = eyafjallajökull.system_info();
	auto devices      = system_info.devices();
	printf("## Found %zu devices:\n\n", std::size(devices));
	std::ranges::for_each(devices,
	                      [](auto const& device) { printf("%s\n", to_string(device).c_str()); });

	auto queue_families = system_info.queue_families();

	if(std::size(queue_families) == 0)
	{
		throw exception{"select graphics devices", "No render queue families are available"};
	}

	printf("\n## Found %zu queue families:\n\n", std::size(queue_families));
	std::ranges::for_each(queue_families,
	                      [](auto const& queue_family)
	                      { printf("%s\n", to_string(queue_family).c_str()); });


	wm::window_base window{1024, 640, "Idis"};
	vk_init::surface surface{eyafjallajökull, window};
	auto usable_devices = collect_usable_devices(queue_families, surface);

	if(std::size(usable_devices) == 0)
	{
		throw exception{"select graphics devices", "No usable graphics device found"};
	}

	printf("\n## Found %zu usable devices:\n\n", std::size(usable_devices));

	std::ranges::for_each(usable_devices,
	                      [](auto const& device) { printf("%s\n", to_string(device).c_str()); });

	std::ranges::sort(usable_devices,
	                  [devices](auto const& a, auto const& b)
	                  {
		                  auto const type_a = devices[a.device_index].properties.deviceType;
		                  auto const type_b = devices[b.device_index].properties.deviceType;
		                  return rank(type_a) < rank(type_b);
	                  });

	auto const& device_info = usable_devices[0];

	printf("\n## Selected device:\n\n%s\n\n", to_string(devices[device_info.device_index]).c_str());


	auto const surface_formats =
	    get_surface_formats(devices[device_info.device_index].device, surface);

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

	auto present_modes =
	    get_surface_present_modes(devices[device_info.device_index].device, surface);
	std::ranges::sort(present_modes, [](auto a, auto b) { return rank(a) < rank(b); });

	auto const present_mode = present_modes[0];
	if(!(present_mode == VK_PRESENT_MODE_MAILBOX_KHR
	     || present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR))
	{
		throw exception{"configure surface", "No suitable present mode found"};
	}

	printf("## Selected present mode %d\n\n", present_mode);

	auto surface_caps = get_surface_capabilities(devices[device_info.device_index].device, surface);

	auto const surface_extent = select_extent(surface_caps, window);

	printf("## Selected surface extent %u x %u\n\n", surface_extent.width, surface_extent.height);

	vk_init::device device{device_info};

	auto graphics_queue = device.get_graphics_queue();
	assert(graphics_queue != nullptr);

	auto surface_queue = device.get_surface_queue();
	assert(surface_queue != nullptr);

	gpu_res::vk_swapchain swapchain{device,
	                                surface,
	                                vk_init::get_image_count(surface_caps),
	                                *surface_format,
	                                surface_extent,
	                                present_mode,
	                                surface_caps.currentTransform};

	auto const images = swapchain.get_images();
	assert(std::size(images) == vk_init::get_image_count(surface_caps));
	return 0;
}
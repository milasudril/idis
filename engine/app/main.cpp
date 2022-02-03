//@	{"target":{"name":"main.o"}}

#include "./main.hpp"

#include "engine/window_manager/vk_instance.hpp"
#include "engine/window_manager/vk_surface.hpp"
#include "engine/window_manager/window.hpp"
#include "engine/window_manager/vk_device.hpp"

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
}

int idis::app::main(int, char**)
{
	printf("# Initiating vulkan\n");
	idis::wm::vk_instance eyafjallajökull;
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


	idis::wm::window_base window{1024, 640, "Idis"};
	idis::wm::vk_surface surface{eyafjallajökull, window};
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

	auto surface_caps = get_surface_capabilities(devices[device_info.device_index].device, surface);
	(void)surface_caps;  // Contains surface size

	auto formats = get_surface_formats(devices[device_info.device_index].device, surface);
	auto present_modes =
	    get_surface_present_modes(devices[device_info.device_index].device, surface);

	idis::wm::vk_device device{device_info};

	auto graphics_queue = device.get_graphics_queue();
	assert(graphics_queue != nullptr);

	auto surface_queue = device.get_surface_queue();
	assert(surface_queue != nullptr);
	return 0;
}
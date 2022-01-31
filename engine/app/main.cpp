//@	{"target":{"name":"main.o"}}

#include "./main.hpp"

#include "engine/window_manager/vk_instance.hpp"
#include "engine/window_manager/window.hpp"

#include <algorithm>
#include <cstdio>

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
	printf("\n## Found %zu queue families:\n\n", std::size(queue_families));
	std::ranges::for_each(queue_families,
	                      [](auto const& queue_family)
	                      { printf("%s\n", to_string(queue_family).c_str()); });


	idis::wm::window_base window{800, 500, "Idis"};
	return 0;
}
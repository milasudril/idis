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

class renderer
{
public:
	explicit renderer(idis::vk_init::device& device,
					  idis::vk_init::surface& surface):
					  m_device{device},
					  m_surface{surface},
					  m_swapchain{device, surface}
	{}

private:
	std::reference_wrapper<idis::vk_init::device> m_device;
	std::reference_wrapper<idis::vk_init::surface> m_surface;
	idis::gpu_res::vk_swapchain m_swapchain;
};

int idis::app::main(int, char**)
{
	crash_handler::arm();

	vk_init::instance eyafjallajökull;
	wm::window_base window{1024, 640, "Idis"};
	vk_init::surface surface{eyafjallajökull, window};
	vk_init::device device{select_device("", eyafjallajökull.system_info(), surface)};

	auto graphics_queue = device.get_graphics_queue();
	assert(graphics_queue != nullptr);

	auto surface_queue = device.get_surface_queue();
	assert(surface_queue != nullptr);

	renderer r{device, surface};
	return 0;
}
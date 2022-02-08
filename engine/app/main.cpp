//@	{"target":{"name":"main.o"}}

#include "./main.hpp"

#include "engine/error_handler/crash_handler.hpp"
#include "engine/vk_init/instance.hpp"
#include "engine/vk_init/surface.hpp"
#include "engine/vk_init/device.hpp"
#include "engine/window_manager/window.hpp"
#include "engine/gpu_res/swapchain.hpp"
#include "engine/gpu_res/image_view.hpp"
#include "engine/gpu_res/utils.hpp"
#include "engine/gpu_res/shader_module.hpp"
#include "engine/shaders/repo.hpp"

#include <algorithm>
#include <cstdio>

#include <cassert>

class renderer
{
public:
	explicit renderer(idis::init::device& device, idis::init::surface& surface)
	    : m_device{device}
	    , m_surface{surface}
	    , m_swapchain{device, surface}
	    , m_img_views{get_image_views_from(m_swapchain)}
	{
		idis::gpu_res::shader_module vert_shader{m_device,
		                                         idis::shaders::repo::get_vertex_shader()};
		idis::gpu_res::shader_module frag_shader{m_device,
		                                         idis::shaders::repo::get_fragment_shader()};

		auto vert_shader_info = get_shader_stage_info(vert_shader);
		printf("%d\n", vert_shader_info.stage);
		auto frag_shader_info = get_shader_stage_info(frag_shader);
		printf("%d\n", frag_shader_info.stage);
	}

private:
	std::reference_wrapper<idis::init::device> m_device;
	std::reference_wrapper<idis::init::surface> m_surface;
	idis::gpu_res::swapchain m_swapchain;
	std::vector<idis::gpu_res::image_view> m_img_views;
};

int idis::app::main(int, char**)
{
	crash_handler::arm();

	init::instance eyafjallajökull;
	wm::window_base window{1024, 640, "Idis"};
	init::surface surface{eyafjallajökull, window};
	init::device device{select_device("", eyafjallajökull.system_info(), surface)};

	auto graphics_queue = device.get_graphics_queue();
	assert(graphics_queue != nullptr);

	auto surface_queue = device.get_surface_queue();
	assert(surface_queue != nullptr);

	renderer r{device, surface};
	return 0;
}
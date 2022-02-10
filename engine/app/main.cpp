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
#include "engine/gpu_res/pipeline_descriptor.hpp"
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
	    , m_shader_prog{
	          {idis::gpu_res::shader_module{m_device, idis::shaders::repo::get_vertex_shader()},
	           idis::gpu_res::shader_module{m_device, idis::shaders::repo::get_fragment_shader()}},
	          idis::gpu_res::pipeline_layout{m_device}}
	{
		m_pipeline.shader_program(m_shader_prog);
	}

private:
	std::reference_wrapper<idis::init::device> m_device;
	std::reference_wrapper<idis::init::surface> m_surface;
	idis::gpu_res::swapchain m_swapchain;
	std::vector<idis::gpu_res::image_view> m_img_views;
	idis::gpu_res::shader_program_info m_shader_prog;
	idis::gpu_res::pipeline_descriptor m_pipeline;
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
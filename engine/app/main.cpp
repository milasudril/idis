//@	{"target":{"name":"main.o"}}

#include "./main.hpp"

#include "engine/error_handler/crash_handler.hpp"
#include "engine/vk_init/instance.hpp"
#include "engine/vk_init/surface.hpp"
#include "engine/vk_init/device.hpp"
#include "engine/window_manager/window.hpp"
#include "engine/gpu_res/vk_swapchain.hpp"
#include "engine/gpu_res/vk_image_view.hpp"
#include "engine/gpu_res/utils.hpp"
#include "engine/gpu_res/vk_shader_module.hpp"
#include "engine/shaders/repo.hpp"

#include <algorithm>
#include <cstdio>

#include <cassert>

class renderer
{
public:
	explicit renderer(idis::vk_init::device& device, idis::vk_init::surface& surface)
	    : m_device{device}
	    , m_surface{surface}
	    , m_swapchain{device, surface}
	    , m_img_views{get_image_views_from(m_swapchain)}
	{
		idis::gpu_res::vk_shader_module<VK_SHADER_STAGE_VERTEX_BIT> vert_shader{
			m_device, idis::shaders::repo::get_vertex_shader()};
		idis::gpu_res::vk_shader_module<VK_SHADER_STAGE_FRAGMENT_BIT> frag_shader{
			m_device, idis::shaders::repo::get_fragment_shader()
		};
	}

private:
	std::reference_wrapper<idis::vk_init::device> m_device;
	std::reference_wrapper<idis::vk_init::surface> m_surface;
	idis::gpu_res::vk_swapchain m_swapchain;
	std::vector<idis::gpu_res::vk_image_view> m_img_views;
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
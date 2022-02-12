//@	{"target":{"name":"main.o"}}

#include "./main.hpp"

#include "engine/error_handler/crash_handler.hpp"
#include "engine/vk_init/instance.hpp"
#include "engine/vk_init/surface.hpp"
#include "engine/vk_init/device.hpp"
#include "engine/window_manager/window.hpp"
#include "engine/gpu_res/command_pool.hpp"
#include "engine/gpu_res/shader_module.hpp"
#include "engine/gpu_res/swapchain.hpp"
#include "engine/gpu_res/image_view.hpp"
#include "engine/gpu_res/render_pass.hpp"
#include "engine/gpu_res/utils.hpp"
#include "engine/gpu_res/pipeline_descriptor.hpp"
#include "engine/gpu_res/pipeline.hpp"
#include "engine/gpu_res/semaphore.hpp"
#include "engine/shaders/repo.hpp"
#include "engine/event_sequencer/event_loop.hpp"
#include "engine/sys/periodic_timer.hpp"
#include "engine/helpers/algext.hpp"


#include <algorithm>
#include <cstdio>
#include <cassert>

namespace
{
	struct window_action_tag
	{
	};

	class renderer
	{
	public:
		explicit renderer(idis::vk_init::device& device, idis::vk_init::surface& surface)
		    : m_device{device}
		    , m_surface{surface}
		    , m_render_finished{device}
		    , m_image_available{device}
		    , m_command_pool{device}
		    , m_shader_prog{
		          {idis::gpu_res::shader_module{m_device, idis::shaders::repo::get_vertex_shader()},
		           idis::gpu_res::shader_module{m_device,
		                                        idis::shaders::repo::get_fragment_shader()}},
		          idis::gpu_res::pipeline_layout{m_device}}
		{
			m_pipeline_info.shader_program(m_shader_prog);
			reconfigure();
		}

		void reconfigure()
		{
			auto new_swapchain = idis::gpu_res::swapchain{m_device, m_surface};
			auto new_img_views = create_image_views_from(new_swapchain);
			auto new_render_pass =
			    idis::gpu_res::render_pass{m_device, new_swapchain.image_format()};
			m_pipeline_info.viewport(new_swapchain.extent()).scissor(new_swapchain.extent());
			auto new_pipeline = idis::gpu_res::pipeline{m_device, m_pipeline_info, new_render_pass};
			auto new_framebuffers = create_framebuffers_from(
			    m_device, new_render_pass, new_swapchain.extent(), new_img_views);
			auto new_command_buffers =
			    idis::gpu_res::command_buffer_set{m_command_pool, std::size(new_framebuffers)};

			idis::for_each(
			    [rp       = new_render_pass.handle(),
			     extent   = new_swapchain.extent(),
			     pipeline = new_pipeline.handle()](auto cmd_buffer, auto const& fb)
			    {
				    idis::gpu_res::command_recording_session rec{cmd_buffer};
				    idis::gpu_res::render_pass_section rp_sec{rec, fb.handle(), rp, extent};
				    rp_sec.bind(cmd_buffer, pipeline).draw(cmd_buffer, 3, 1, 0, 0);
			    },
			    new_command_buffers.buffers(),
			    new_framebuffers);

			m_swapchain       = std::move(new_swapchain);
			m_img_views       = std::move(new_img_views);
			m_render_pass     = std::move(new_render_pass);
			m_pipeline        = std::move(new_pipeline);
			m_framebuffers    = std::move(new_framebuffers);
			m_command_buffers = std::move(new_command_buffers);
		}

		void draw_frame() { puts("Hej"); }

	private:
		std::reference_wrapper<idis::vk_init::device> m_device;
		std::reference_wrapper<idis::vk_init::surface> m_surface;
		idis::gpu_res::semaphore m_render_finished;
		idis::gpu_res::semaphore m_image_available;
		idis::gpu_res::command_pool m_command_pool;
		idis::gpu_res::shader_program_info m_shader_prog;
		idis::gpu_res::pipeline_descriptor m_pipeline_info;

		idis::gpu_res::swapchain m_swapchain;
		std::vector<idis::gpu_res::image_view> m_img_views;
		idis::gpu_res::render_pass m_render_pass;
		idis::gpu_res::pipeline m_pipeline;
		std::vector<idis::gpu_res::framebuffer> m_framebuffers;
		idis::gpu_res::command_buffer_set m_command_buffers;
	};

	struct engine_state
	{
		idis::seq::event_loop loop;
	};

	void window_closed(engine_state& obj, window_action_tag) { obj.loop.state().set_exit_flag(); }
}

int idis::app::main(int, char**)
{
	crash_handler::arm();
	vk_init::instance eyafjallajökull;
	engine_state state;
	wm::window mainwin{state, 1024, 640, "Idis"};
	mainwin.set_close_callback<window_action_tag>();
	//TODO: .set_size_callback<window_action_tag>();
	vk_init::surface surface{eyafjallajökull, mainwin};
	vk_init::device device{select_device("", eyafjallajökull.system_info(), surface)};

	auto graphics_queue = device.get_graphics_queue();
	assert(graphics_queue != nullptr);

	auto surface_queue = device.get_surface_queue();
	assert(surface_queue != nullptr);

	renderer r{device, surface};

	state.loop.set_pre_drain_callback(glfwPollEvents);
	state.loop.set_post_drain_callback(
	    [timer = idis::sys::periodic_timer{idis::seq::seconds_per_tick}, &r]() mutable
	    {
		    r.draw_frame();
		    timer.wait();
	    });
	state.loop.run();
	vkDeviceWaitIdle(device.handle());

	return 0;
}
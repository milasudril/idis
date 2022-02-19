//@	{"target":{"name":"main.o"}}

#include "./main.hpp"

#include "engine/error_handling/crash_handler.hpp"
#include "engine/vk_init/instance.hpp"
#include "engine/vk_init/surface.hpp"
#include "engine/vk_init/device.hpp"
#include "engine/window_managment/window.hpp"
#include "engine/gpu_res/command_pool.hpp"
#include "engine/gpu_res/shader_module.hpp"
#include "engine/gpu_res/swapchain.hpp"
#include "engine/gpu_res/image_view.hpp"
#include "engine/gpu_res/render_pass.hpp"
#include "engine/gpu_res/utils.hpp"
#include "engine/gpu_res/pipeline_descriptor.hpp"
#include "engine/gpu_res/pipeline.hpp"
#include "engine/gpu_res/semaphore.hpp"
#include "engine/gpu_res/fence.hpp"
#include "engine/shaders/repo.hpp"
#include "engine/gpu_res/buffer.hpp"
#include "engine/event_sequencer/event_loop.hpp"
#include "engine/sys/periodic_timer.hpp"
#include "engine/utils/algext.hpp"
#include "engine/utils/pair.hpp"
#include "engine/vk_init/allocator.hpp"
#include "engine/shaders/testprog.hpp"

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
		explicit renderer(idis::vk_init::device& device,
		                  idis::vk_init::surface& surface,
		                  idis::vk_init::allocator& allocator)
		    : m_device{device}
		    , m_surface{surface}
		    , m_allocator{allocator}
		    , m_force_reconfigure{true}
		    , m_frame_index{0}
		    , m_render_fence{idis::gpu_res::fence{device}, idis::gpu_res::fence{device}}
		    , m_image_available{idis::gpu_res::semaphore{device}}
		    , m_render_finished{idis::gpu_res::semaphore{device}}
		    , m_command_pool{device}
		    , m_command_buffers{idis::gpu_res::command_buffer_set{m_command_pool,
		                                                          std::size(m_render_fence)}}
		    , m_shader_prog{{idis::gpu_res::shader_module{m_device,
		                                                  idis::shaders::repo::get_vertex_shader()},
		                     idis::gpu_res::shader_module{
		                         m_device, idis::shaders::repo::get_fragment_shader()}},
		                    idis::gpu_res::pipeline_layout{m_device}}
		    , m_vbo{idis::gpu_res::vertex_buffer<
		          VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT>(m_allocator.get(), 12)}
		{
			std::array<idis::pair<float>, 3> const verts{
			    idis::pair{0.5f, -0.5f}, idis::pair{0.5f, 0.5f}, idis::pair{-0.5f, 0.5f}};
			sync_transfer(m_vbo, std::span{verts});
			m_pipeline_info.shader_program(m_shader_prog);
		}

		void reconfigure()
		{
			m_device.get().wait();

			// Must remove all resources. Otherwise, the window is busy.
			m_framebuffers.clear();
			m_pipeline.reset();
			m_render_pass.reset();
			m_img_views.clear();
			m_swapchain.reset();

			auto new_swapchain = idis::gpu_res::swapchain{m_device, m_surface};
			auto new_img_views = create_image_views_from(new_swapchain);
			auto new_render_pass =
			    idis::gpu_res::render_pass{m_device, new_swapchain.image_format()};
			m_pipeline_info.viewport(new_swapchain.extent()).scissor(new_swapchain.extent());
			auto new_pipeline = idis::gpu_res::pipeline{m_device, m_pipeline_info, new_render_pass};
			auto new_framebuffers =
			    create_framebuffers_from(new_render_pass, new_swapchain.extent(), new_img_views);

			m_swapchain    = std::move(new_swapchain);
			m_img_views    = std::move(new_img_views);
			m_render_pass  = std::move(new_render_pass);
			m_pipeline     = std::move(new_pipeline);
			m_framebuffers = std::move(new_framebuffers);
		}

		void draw_frame()
		{
			if(m_force_reconfigure)
			{
				reconfigure();
				m_force_reconfigure = false;
			}

			auto const k = m_frame_index % std::size(m_render_fence);

			wait_and_reset(m_render_fence[k]);
			auto const img_index =
			    acquire_next_image(m_device,
			                       m_swapchain,
			                       idis::gpu_res::signal_semaphore{m_image_available},
			                       [this]() { reconfigure(); });

			{
				auto rp         = m_render_pass.handle();
				auto extent     = m_swapchain.extent();
				auto cmd_buffer = m_command_buffers[k];
				auto const& fb  = m_framebuffers[img_index];
				idis::gpu_res::command_recording_session rec{cmd_buffer};
				idis::gpu_res::render_pass_section rp_sec{rec, fb.handle(), rp, extent};
				rp_sec.bind(cmd_buffer, m_pipeline.handle()).draw(cmd_buffer, 3, 1, 0, 0);
			}

			submit(m_device.get().graphics_queue(),
			       m_command_buffers[k],
			       idis::gpu_res::wait_semaphore{m_image_available},
			       idis::gpu_res::signal_semaphore{m_render_finished},
			       idis::gpu_res::signal_fence{m_render_fence[k]});
			present(m_device.get().surface_queue(),
			        m_swapchain,
			        img_index,
			        idis::gpu_res::wait_semaphore{m_render_finished});

			++m_frame_index;
		}

		void reconfigure_next_frame() { m_force_reconfigure = true; }

	private:
		std::reference_wrapper<idis::vk_init::device const> m_device;
		std::reference_wrapper<idis::vk_init::surface const> m_surface;
		std::reference_wrapper<idis::vk_init::allocator const> m_allocator;
		bool m_force_reconfigure;
		size_t m_frame_index;
		std::array<idis::gpu_res::fence, 2> m_render_fence;
		idis::gpu_res::semaphore m_image_available;
		idis::gpu_res::semaphore m_render_finished;

		idis::gpu_res::command_pool m_command_pool;
		idis::gpu_res::command_buffer_set m_command_buffers;
		idis::gpu_res::shader_program_info m_shader_prog;
		idis::gpu_res::vertex_buffer<VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT> m_vbo;
		idis::gpu_res::pipeline_descriptor m_pipeline_info;

		idis::gpu_res::swapchain m_swapchain;
		std::vector<idis::gpu_res::image_view> m_img_views;
		idis::gpu_res::render_pass m_render_pass;
		idis::gpu_res::pipeline m_pipeline;
		std::vector<idis::gpu_res::framebuffer> m_framebuffers;
	};

	struct engine_state
	{
		idis::seq::event_loop loop;
		renderer* r;
	};

	void window_closed(engine_state& obj, window_action_tag) { obj.loop.state().set_exit_flag(); }

	void window_size_changed(engine_state& obj, window_action_tag, idis::wm::dimensions)
	{
		if(obj.r != nullptr) { obj.r->reconfigure_next_frame(); }
	}
}

int idis::app::main(int, char**)
{
	crash_handler::arm();
	vk_init::instance eyafjallajökull;
	engine_state state;
	wm::window mainwin{state, 1024, 640, "Idis"};
	mainwin.set_close_callback<window_action_tag>();
	mainwin.set_size_callback<window_action_tag>();
	vk_init::surface surface{eyafjallajökull, mainwin};
	vk_init::device device{select_device("", eyafjallajökull.system_info(), surface)};
	vk_init::allocator alloc{device, eyafjallajökull.handle()};

	renderer r{device, surface, alloc};
	state.r = &r;

	state.loop.set_pre_drain_callback(glfwPollEvents);
	state.loop.set_post_drain_callback(
	    [timer = idis::sys::periodic_timer{idis::seq::seconds_per_tick}, &r]() mutable
	    {
		    r.draw_frame();
		    timer.wait();
	    });
	state.loop.run();
	device.wait();

	return 0;
}
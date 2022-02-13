//@	{"target":{"name":"idis.o"}}

#include "./main.hpp"

#include "engine/window_manager/window.hpp"
#include "engine/sw_graphics/cairo_surface.hpp"
#include "engine/event_sequencer/event_loop.hpp"
#include "engine/sys/periodic_timer.hpp"
#include "engine/sys/child_proc.hpp"
#include "engine/utils/algext.hpp"

#include "fruit/lib/text_box.hpp"
#include "fruit/lib/font_mapper.hpp"
#include "fruit/lib/font_face.hpp"
#include "fruit/lib/io_utils.hpp"

#include <cstdio>

namespace
{
	struct window_action_tag
	{
	};

	struct message_display
	{
		idis::seq::event_loop event_loop;
		idis::sw_graphics::cairo_surface draw_surface;

		message_display(std::reference_wrapper<fruit::FontFace const> font): message{font} {}

		std::string msg;
		fruit::TextBox message;
	};

	void window_closed(message_display& obj, window_action_tag)
	{
		obj.event_loop.state().set_exit_flag();
	}

	void render(message_display& obj, idis::wm::dimensions dim)
	{
		obj.draw_surface.fill(0x00, 0x00, 0x66);
		pixel_store::image<pixel_store::rgba_value<>> img{static_cast<uint32_t>(dim.width),
		                                                  static_cast<uint32_t>(dim.height)};

		obj.message.char_height(dim.height / 32);
		// TODO: Wordwrap
		// auto res = obj.message.handle(fruit::SizeRequestEvent{});
		obj.message.compose(
		    img.pixels(), fruit::Point{0, 0, 0}, fruit::Pixel{0.66f, 0.66f, 0.66f, 0.0f});
		idis::sw_graphics::cairo_image_surface surface{std::as_const(img).pixels()};
		obj.draw_surface.fill(surface, pixel_store::vec4_t<int>{0, 0, 0, 0});
	}

	void window_size_changed(message_display& obj, window_action_tag, idis::wm::dimensions dim)
	{
		obj.draw_surface.set_dimensions(dim);
		render(obj, dim);
	}

	void present(std::exception const& e)
	try
	{
		fruit::FontMapper font_mapper;
		auto font_file = font_mapper.get_path("mono-space");
		fruit::FontfaceLoader font_loader;
		fruit::FontFace font_face{font_loader, fruit::io_utils::load(font_file)};
		message_display md{font_face};
		md.msg = e.what();
		md.message.text(idis::split<std::basic_string<char8_t>>(md.msg, '\n'));
		idis::wm::window mainwin{md, 1024, 640, "Idis"};
		mainwin.set_close_callback<window_action_tag>().set_size_callback<window_action_tag>();
		md.event_loop.set_pre_drain_callback(glfwPollEvents);
		md.draw_surface = idis::sw_graphics::cairo_surface{mainwin};
		render(md, mainwin.get_dimensions());
		md.event_loop.set_post_drain_callback(
		    [timer = idis::sys::periodic_timer{idis::seq::seconds_per_tick}]() mutable
		    { timer.wait(); });
		md.event_loop.run();
	}
	catch(...)
	{
		fprintf(stderr, "idis: %s\n", e.what());
	}
}

int main(int argc, char** argv)
try
{
	auto res = idis::sys::child_proc{"idis", idis::app::main, argc, argv}.get_result();

	if(has_error(res)) { throw std::runtime_error{to_string(res)}; }
}
catch(std::exception const& e)
{
	present(e);
	return -1;
}
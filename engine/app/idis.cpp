//@	{"target":{"name":"idis.o"}}

#include "engine/window_manager/window.hpp"
#include "engine/window_manager/cairo_surface.hpp"
#include "engine/event_sequencer/event_loop.hpp"

#include <cstdio>

struct window_action_tag
{
};

struct message_display
{
	idis::seq::event_loop event_loop;
	idis::wm::cairo_surface draw_surface;
};

void window_closed(message_display& obj, window_action_tag)
{
	obj.event_loop.state().set_exit_flag();
}

void window_size_changed(message_display& obj, window_action_tag, idis::wm::dimensions dim)
{
	obj.draw_surface.set_dimensions(dim).fill(0x00, 0x00, 0xaa);
}

void present(std::exception const& e)
try
{
	message_display md;
	idis::wm::window mainwin{md, 800, 500, "Idis"};
	mainwin.set_close_callback<window_action_tag>().set_size_callback<window_action_tag>();
	md.event_loop.set_pre_drain_callback(glfwWaitEvents);
	md.draw_surface = idis::wm::cairo_surface{mainwin};
	md.draw_surface.fill(0x00, 0x00, 0xaa);
	md.event_loop.run();
}
catch(...)
{
	fprintf(stderr, "idis: %s\n", e.what());
}

int main(int, char**)
try
{
	idis::wm::window_base window{800, 500, "Idis"};
	throw idis::exception{"start app", "not implemented"};
}
catch(std::exception const& e)
{
	present(e);
	return -1;
}
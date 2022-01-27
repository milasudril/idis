//@	{"target":{"name":"idis.o"}}

#include "./window_manager/window.hpp"
#include "./window_manager/cairo_surface.hpp"
#include "./event_sequencer/event_loop.hpp"

#include <cstdio>

struct window_action_tag
{
};

void window_closed(idis::seq::event_loop_state& loop_state, window_action_tag)
{
	loop_state.set_exit_flag();
}

void present(std::exception const& e)
try
{
	idis::seq::event_loop loop;
	idis::wm::window window{800, 500, "Idis"};
	loop.set_pre_drain_callback(glfwPollEvents);
	idis::wm::cairo_surface surface{window};
	window.set_event_handler(loop.state()).set_close_callback<window_action_tag>();
	loop.run();
}
catch(...)
{
	fprintf(stderr, "idis: %s\n", e.what());
}

int main(int, char**)
try
{
	idis::wm::window window{800, 500, "Idis"};
	throw idis::exception{"start app", "not implemented"};
}
catch(std::exception const& e)
{
	present(e);
	return -1;
}
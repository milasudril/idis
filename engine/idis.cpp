//@	{"target":{"name":"idis.o"}}

#include "./window_manager/window.hpp"
#include "./event_sequencer/event_loop.hpp"

#include <cstdio>

struct event_handler
{
	std::reference_wrapper<idis::seq::event_loop_state> loop_state;
};

void window_closed(event_handler eh)
{
	eh.loop_state.get().set_exit_flag();
}

void present(std::exception const& e)
try
{
	idis::seq::event_loop loop;
	idis::wm::window<event_handler> window{800, 500, "Idis", event_handler{loop.state()}};
	window.enable_close_callback();
	loop.set_pre_drain_callback(glfwPollEvents);
//	window.show_pixels();
	loop.run();
}
catch(...)
{
	fprintf(stderr, "idis: %s\n", e.what());
}

int main(int, char**)
try
{
	idis::wm::window<int> window{800, 500, "Idis"};
	throw idis::exception{"start app", "not implemented"};
}
catch(std::exception const& e)
{
	present(e);
	return -1;
}
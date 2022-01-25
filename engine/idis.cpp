//@	{"target":{"name":"idis.o"}}

#include "./window_manager/window.hpp"
#include "./event_sequencer/event_loop.hpp"

#include <cstdio>

void present(std::exception const& e)
try
{
	idis::wm::window window{800, 500, "Idis"};
	idis::seq::event_loop loop;
	loop.set_pre_drain_callback(glfwPollEvents);
	window.show_pixels();
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
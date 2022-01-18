//@	{"target":{"name":"idis.o"}}

#include "./window_manager/window.hpp"

#include <cstdio>

int main(int, char**)
try
{
	idis::wm::window idis_window{800, 500, "Idis"};
	while(true)
	{
		glfwPollEvents();
	}
}
catch(std::exception const& e)
{
	fprintf(stderr, "idis: %s\n", e.what());
}
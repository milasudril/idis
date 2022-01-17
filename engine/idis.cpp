//@	{"target":{"name":"idis.o"}}

#include "./windowing_system/window_manager.hpp"

#include <cstdio>

int main(int, char**)
try
{
	idis::window_manager wm;
	printf("Initialized %s %s\n", wm.driver(), wm.version_string());
}
catch(std::exception const& e)
{
	fprintf(stderr, "idis: %s\n", e.what());
}
//@	{"target":{"name":"idis.o"}}

#include "./window_manager/window.hpp"

#include <cstdio>

void present(std::exception const& e)
try
{
	idis::wm::window window{800, 500, "Idis"};
	throw idis::exception{"show error message", "not implemented"};
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
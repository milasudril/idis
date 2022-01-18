//@	{"target":{"name":"idis.o"}}

#include "./window_manager/initializer.hpp"

#include <cstdio>

int main(int, char**)
try
{
	idis::wm::initializer init;
}
catch(std::exception const& e)
{
	fprintf(stderr, "idis: %s\n", e.what());
}
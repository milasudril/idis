//@	{"target":{"name":"idis.o"}}

#include "./error_handler/exception.hpp"

#include <cstdio>

int main(int, char**)
try
{
	throw idis::exception{"start idis", "Not implemented"};
}
catch(std::exception const& e)
{
	fprintf(stderr, "idis: %s", e.what());
}
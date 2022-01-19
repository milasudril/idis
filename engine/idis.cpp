//@	{"target":{"name":"idis.o"}}

#include "./window_manager/window.hpp"

#include <cstdio>

template<class Function, class... Args>
void with_window(Function&& f, Args&&... args)
try
{
	idis::wm::window window{800, 500, "Idis"};
	f(window, std::forward<Args>(args)...);
}
catch(std::exception const& e)
{
	fprintf(stderr, "idis: %s\n", e.what());
}

int main(int, char**)
{
	with_window(
	    [](auto&)
	    {
		    try
		    {
		    }
		    catch(std::exception const& e)
		    {
			    with_window([](auto&, char const*) {}, e.what());
		    }
	    });
}
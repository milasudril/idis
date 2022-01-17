#ifndef IDIS_WINDOWINGSYSTEM_WINDOWMANAGER_HPP
#define IDIS_WINDOWINGSYSTEM_WINDOWMANAGER_HPP

#include "error_handler/exception.hpp"

namespace idis
{
	class window_manager
	{
	public:
		window_manager()
		{
			throw exception{"initialize window manager", "not implemented"};
		}

		window_manager(window_manager const&)=delete;
		window_manager(window_manager&&)=delete;
		window_manager& operator=(window_manager const&)=delete;
		window_manager& operator=(window_manager&&)=delete;

		~window_manager()
		{}
	};
}

#endif
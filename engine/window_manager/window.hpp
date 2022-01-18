//@{"dependencies_extra":[{"ref":"glfw3", "rel":"implementation", "origin":"pkg-config"}]}

#ifndef IDIS_WINDOWMANAGER_WINDOW_HPP
#define IDIS_WINDOWMANAGER_WINDOW_HPP

#include "./initializer.hpp"
#include "error_handler/exception.hpp"

#include <GLFW/glfw3.h>

namespace idis::wm
{
	class window
	{
	public:
		window()
		{
		}

	private:
		[[no_unique_address]] initializer m_init;
	};
}

#endif
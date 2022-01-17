//@{"dependencies_extra":[{"ref":"glfw3", "rel":"implementation", "origin":"pkg-config"}]}

#ifndef IDIS_WINDOWINGSYSTEM_WINDOWMANAGER_HPP
#define IDIS_WINDOWINGSYSTEM_WINDOWMANAGER_HPP

#include "error_handler/exception.hpp"

#include <GLFW/glfw3.h>

namespace idis
{
	class window_manager
	{
	public:
		window_manager()
		{
			if(glfwInit() != GLFW_TRUE) [[unlikely]]
			{
				char const* cause = nullptr;
				glfwGetError(&cause);
				throw exception{"initialize the window manager",
				                cause != nullptr ? cause : "Unknown error"};
			}
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}

		char const* version_string() const { return glfwGetVersionString(); }

		static constexpr char const* driver() { return "GLFW"; }

		window_manager(window_manager const&) = delete;
		window_manager(window_manager&&)      = delete;
		window_manager& operator=(window_manager const&) = delete;
		window_manager& operator=(window_manager&&) = delete;

		~window_manager() { glfwTerminate(); }
	};
}

#endif
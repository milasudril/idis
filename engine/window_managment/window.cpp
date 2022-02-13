//@	{
//@ "target":{"name":"window.o"}
//@	}

#include "./window.hpp"

#include "engine/error_handling/exception.hpp"
#include "engine/error_handling/common_causes.hpp"

idis::wm::window_base::window_base(int width, int height, char const* title)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_handle = handle_type{glfwCreateWindow(width, height, title, nullptr, nullptr)};
	if(m_handle == nullptr) [[unlikely]]
	{
		char const* cause = nullptr;
		glfwGetError(&cause);
		throw exception{"Failed to create a new window",
		                cause != nullptr ? cause : idis::unknown_error};
	}
}

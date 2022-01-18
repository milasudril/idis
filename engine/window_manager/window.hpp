//@{"dependencies_extra":[{"ref":"glfw3", "rel":"implementation", "origin":"pkg-config"}]}

#ifndef IDIS_WINDOWMANAGER_WINDOW_HPP
#define IDIS_WINDOWMANAGER_WINDOW_HPP

#include "./initializer.hpp"
#include "error_handler/exception.hpp"

#include <GLFW/glfw3.h>

#include <utility>

namespace idis::wm
{
	class window
	{
	public:
		explicit window(int width, int height, char const* title)
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			m_handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
		}


		window(window&& other) noexcept { m_handle = std::exchange(other.m_handle, nullptr); }

		window(window const&) = delete;
		window& operator=(window const&) = delete;

		~window() noexcept
		{
			if(m_handle != nullptr) { glfwDestroyWindow(m_handle); }
		}

		window& operator=(window&& other) noexcept
		{
			glfwDestroyWindow(m_handle);
			m_handle = std::exchange(other.m_handle, nullptr);
			return *this;
		}

	private:
		[[no_unique_address]] initializer m_init;
		GLFWwindow* m_handle;
	};
}

#endif
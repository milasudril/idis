//@	{
//@ "dependencies_extra":[
//@ 	 {"ref":"glfw3", "rel":"implementation", "origin":"pkg-config"}
//@ 	,{"ref":"./window.o", "rel":"implementation"}
//@		]
//@	}

#ifndef IDIS_WINDOWMANAGER_WINDOW_HPP
#define IDIS_WINDOWMANAGER_WINDOW_HPP

#include "./initializer.hpp"
#include "error_handler/exception.hpp"

#include <GLFW/glfw3.h>

#include <utility>

namespace idis::wm
{
	template<class EventHandler>
	class window
	{
	public:
		explicit window(int width, int height, char const* title, EventHandler&& event_handler = EventHandler{})
		:m_event_handler{std::move(event_handler)}
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			m_handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
			if(m_handle == nullptr) [[unlikely]]
			{
				char const* cause = nullptr;
				glfwGetError(&cause);
				throw exception{"Failed to create a new window",
				                cause != nullptr ? cause : "Unknown error"};
			}
			glfwSetWindowUserPointer(m_handle, &m_event_handler);
		}

		window(window&& other) noexcept = delete;
		window(window const&) = delete;
		window& operator=(window const&) = delete;
		window& operator=(window&& other) noexcept = delete;

		~window() noexcept
		{
			if(m_handle != nullptr) { glfwDestroyWindow(m_handle); }
		}
/*
//TODO
		window& show_pixels();
*/
		window& set_callback_data(EventHandler&& callback_data)
		{
			m_event_handler = std::move(callback_data);
			return *this;
		}

		window& enable_close_callback()
		{
			glfwSetWindowCloseCallback(m_handle, [](GLFWwindow* window){
				auto data = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
				window_closed(*data);
			});
			return *this;
		}


	private:
		[[no_unique_address]] initializer m_init;
		GLFWwindow* m_handle;
		EventHandler m_event_handler;
	};

}

#endif
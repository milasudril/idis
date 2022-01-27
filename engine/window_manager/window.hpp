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
#include <memory>

namespace idis::wm
{
	template<class T>
	class callback_registrator
	{
	public:
		explicit callback_registrator(GLFWwindow* handle, T& obj): m_handle{handle}
		{
			glfwSetWindowUserPointer(handle, &obj);
		}

		template<class Tag>
		callback_registrator& set_close_callback()
		{
			glfwSetWindowCloseCallback(m_handle,
			                           [](GLFWwindow* handle)
			                           {
				                           auto& obj =
				                               *static_cast<T*>(glfwGetWindowUserPointer(handle));
				                           window_closed(obj, Tag{});
			                           });
			return *this;
		}

	private:
		GLFWwindow* m_handle;
	};

	namespace detail
	{
		struct window_deleter
		{
			void operator()(GLFWwindow* window) const
			{
				if(window != nullptr) { glfwDestroyWindow(window); }
			}
		};
	}

	class window
	{
		using handle_type = std::unique_ptr<GLFWwindow, detail::window_deleter>;

	public:
		explicit window(int width, int height, char const* title)
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			m_handle = handle_type{glfwCreateWindow(width, height, title, nullptr, nullptr)};
			if(m_handle == nullptr) [[unlikely]]
			{
				char const* cause = nullptr;
				glfwGetError(&cause);
				throw exception{"Failed to create a new window",
				                cause != nullptr ? cause : "Unknown error"};
			}
		}

		template<class T>
		[[nodiscard]] auto set_event_handler(T& obj)
		{
			return callback_registrator{m_handle.get(), obj};
		}

		auto handle()
		{ return m_handle.get(); }

	private:
		[[no_unique_address]] initializer m_init;
		handle_type m_handle;
	};
}

#endif
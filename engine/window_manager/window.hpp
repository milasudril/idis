//@	{
//@ "dependencies_extra":
//@		[
//@		{"ref":"./window.o", "rel":"implementation"},
//@		{"ref":"glfw3", "rel":"implementation", "origin":"pkg-config"}
//@		]
//@	}

#ifndef IDIS_WINDOWMANAGER_WINDOW_HPP
#define IDIS_WINDOWMANAGER_WINDOW_HPP

#include "./initializer.hpp"

#include <GLFW/glfw3.h>

#include <utility>
#include <memory>

namespace idis::wm
{
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

	struct dimensions
	{
		int width;
		int height;
	};

	class window_base
	{
		using handle_type = std::unique_ptr<GLFWwindow, detail::window_deleter>;

	public:
		explicit window_base(int width, int height, char const* title);

		auto handle() { return m_handle.get(); }

		dimensions get_dimensions() const
		{
			dimensions ret{};
			glfwGetWindowSize(m_handle.get(), &ret.width, &ret.height);
			return ret;
		}

		dimensions get_fb_dimensions() const
		{
			dimensions ret{};
			glfwGetFramebufferSize(m_handle.get(), &ret.width, &ret.height);
			return ret;
		}

	private:
		[[no_unique_address]] initializer m_init;
		handle_type m_handle;
	};

	template<class EventHandler>
	class window final: public window_base
	{
	public:
		template<class... Args>
		window(EventHandler& eh, Args&&... args): window_base{std::forward<Args>(args)...}
		{
			set_event_handler(eh);
		}

		window& set_event_handler(EventHandler& eh)
		{
			glfwSetWindowUserPointer(handle(), &eh);
			return *this;
		}

		template<class Tag>
		window& set_close_callback()
		{
			glfwSetWindowCloseCallback(handle(),
			                           [](GLFWwindow* handle)
			                           {
				                           auto& eh = *static_cast<EventHandler*>(
				                               glfwGetWindowUserPointer(handle));
				                           window_closed(eh, Tag{});
			                           });
			return *this;
		}

		template<class Tag>
		window& set_size_callback()
		{
			glfwSetWindowSizeCallback(handle(),
			                          [](GLFWwindow* handle, int width, int height)
			                          {
				                          auto& eh = *static_cast<EventHandler*>(
				                              glfwGetWindowUserPointer(handle));
				                          window_size_changed(eh, Tag{}, dimensions{width, height});
			                          });
			return *this;
		}
	};
}

#endif
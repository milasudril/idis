//@	{
//@ "dependencies_extra":
//@		[
//@		{"ref":"./window.o", "rel":"implementation"},
//@		{"ref":"glfw3", "rel":"implementation", "origin":"pkg-config"}
//@		]
//@	}

#ifndef IDIS_WINDOWMANAGMENT_WINDOW_HPP
#define IDIS_WINDOWMANAGMENT_WINDOW_HPP

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

		auto handle() const { return m_handle.get(); }

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

	inline auto select_extent(VkSurfaceCapabilitiesKHR const& caps, wm::window_base const& window)
	{
		if(caps.currentExtent.width != UINT32_MAX) { return caps.currentExtent; }

		auto const dim = window.get_fb_dimensions();

		return VkExtent2D{std::clamp(static_cast<uint32_t>(dim.width),
		                             caps.minImageExtent.width,
		                             caps.maxImageExtent.width),
		                  std::clamp(static_cast<uint32_t>(dim.height),
		                             caps.minImageExtent.height,
		                             caps.maxImageExtent.height)};
	}
}

#endif
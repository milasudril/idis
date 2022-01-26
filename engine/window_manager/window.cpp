//@	{
//@ "target":{"name":"window.o", "pkgconfig_libs":["x11"]}
//@	}

#include "./window.hpp"

#define GLFW_EXPOSE_NATIVE_X11

#include <GLFW/glfw3native.h>
#include <cstring>
#include <memory>

namespace
{
	struct XImageDeleter
	{
		void operator()(XImage* img) { XDestroyImage(img); }
	};
}

idis::wm::window& idis::wm::window::show_pixels()
{
	auto const disp   = glfwGetX11Display();
	auto const win    = glfwGetX11Window(m_handle);
	auto const screen = DefaultScreen(disp);                             // NOLINT
	auto buffer       = reinterpret_cast<char*>(malloc(256 * 256 * 4));  // NOLINT
	memset(buffer, 0xff, 256 * 256 * 4);
	//NOLINTNEXTLINE
	std::unique_ptr<XImage, XImageDeleter> img{
	    XCreateImage(disp,                                      // NOLINT
	                 DefaultVisual(disp, DefaultScreen(disp)),  // NOLINT
	                 DefaultDepth(disp, screen),                // NOLINT
	                 ZPixmap,
	                 0,
	                 buffer,
	                 256,
	                 256,
	                 32,
	                 0)};
	XPutImage(disp,
	          win,
	          DefaultGC(disp, screen),  // NOLINT
	          img.get(),
	          0,
	          0,
	          0,
	          0,
	          256,
	          256);
	return *this;
}

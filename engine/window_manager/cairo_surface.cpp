//@	{"target":{"name":"cairo_surface.o"}}

#include "./cairo_surface.hpp"

idis::wm::cairo_surface::cairo_surface(window_base& target)
{
	auto x11_disp = glfwGetX11Display();
	auto x11_win  = glfwGetX11Window(target.handle());
	auto screen   = DefaultScreen(x11_disp);
	auto dim      = target.get_dimensions();
	m_surface     = surface_type{cairo_xlib_surface_create(
        x11_disp, x11_win, DefaultVisual(x11_disp, screen), dim.width, dim.height)};
	cairo_xlib_surface_set_size(m_surface.get(), dim.width, dim.height);
	m_ctxt = ctxt_type{cairo_create(m_surface.get())};
}
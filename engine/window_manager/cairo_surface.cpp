//@	{"target":{"name":"cairo_surface.o"}}

#include "./cairo_surface.hpp"

idis::wm::cairo_image_surface::cairo_image_surface(
    pixel_store::image_span<pixel_store::rgba_value<> const> buffer)
    : m_handle{cairo_image_surface_create(
        CAIRO_FORMAT_ARGB32, static_cast<int>(buffer.width()), static_cast<int>(buffer.height()))}
{
}

idis::wm::cairo_surface::cairo_surface(window_base& target)
{
	auto x11_disp = glfwGetX11Display();
	auto x11_win  = glfwGetX11Window(target.handle());
	auto screen   = DefaultScreen(x11_disp);
	m_dim         = target.get_dimensions();
	m_surface     = surface_type{cairo_xlib_surface_create(
        x11_disp, x11_win, DefaultVisual(x11_disp, screen), m_dim.width, m_dim.height)};
	cairo_xlib_surface_set_size(m_surface.get(), m_dim.width, m_dim.height);
	m_ctxt = ctxt_type{cairo_create(m_surface.get())};
}

//@	{"target":{"name":"cairo_surface.o"}}

#include "./cairo_surface.hpp"

idis::wm::cairo_image_surface::cairo_image_surface(
    pixel_store::image_span<pixel_store::rgba_value<> const> img)
    : m_handle{cairo_image_surface_create(
        CAIRO_FORMAT_ARGB32, static_cast<int>(img.width()), static_cast<int>(img.height()))}
        , m_dim{static_cast<int>(img.width()), static_cast<int>(img.height())}
{
	auto const stride = cairo_image_surface_get_stride(handle());
	cairo_surface_flush(handle());
 	auto const output_buffer = cairo_image_surface_get_data(handle());
	auto read_ptr = img.data();
	for(size_t k = 0; k != img.height(); ++k)
	{
		auto write_ptr = output_buffer + k * stride;
		for(size_t l = 0; l != img.width(); ++l)
		{
			// TODO: Use look-up table for gamma correction
			// TODO: Handle out-of-range values

			auto const v = pixel_store::vector_cast<uint8_t>(255.0f*img(l, k).value());

			write_ptr[0] = v[2];
			write_ptr[1] = v[1];
			write_ptr[2] = v[0];
			write_ptr[3] = v[3];

			++read_ptr;
			write_ptr += 4;
		}
	}
	cairo_surface_mark_dirty(handle());
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

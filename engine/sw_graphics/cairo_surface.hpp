//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./cairo_surface.o", "rel":"implementation"},
//@		{"ref":"cairo", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_WINDOWMANAGER_CAIROSURFACE_HPP
#define IDIS_WINDOWMANAGER_CAIROSURFACE_HPP

#include "engine/window_manager/window.hpp"

#include "pixel_store/image.hpp"
#include "pixel_store/rgba_value.hpp"

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

namespace idis::wm
{
	namespace detail
	{
		struct cairo_deleter
		{
			void operator()(cairo_t* ctxt) const
			{
				if(ctxt != nullptr) { cairo_destroy(ctxt); }
			}

			void operator()(cairo_surface_t* surf) const
			{
				if(surf != nullptr) { cairo_surface_destroy(surf); }
			}
		};
	}

	class cairo_image_surface
	{
		using surface_type = std::unique_ptr<cairo_surface_t, detail::cairo_deleter>;

	public:
		explicit cairo_image_surface(
		    pixel_store::image_span<pixel_store::rgba_value<> const> buffer);

		auto handle() const { return m_handle.get(); }

		auto get_dimensions() const { return m_dim; }

	private:
		surface_type m_handle;
		dimensions m_dim;
	};

	class cairo_surface
	{
		using ctxt_type    = std::unique_ptr<cairo_t, detail::cairo_deleter>;
		using surface_type = std::unique_ptr<cairo_surface_t, detail::cairo_deleter>;

	public:
		cairo_surface() = default;

		explicit cairo_surface(window_base& target_window);

		cairo_surface& fill(uint8_t r, uint8_t g, uint8_t b)
		{
			cairo_set_source_rgba(m_ctxt.get(), r / 255.0, g / 255.0, b / 255.0, 1.0);
			cairo_rectangle(m_ctxt.get(), 0, 0, m_dim.width, m_dim.height);
			cairo_fill(m_ctxt.get());
			return *this;
		}

		cairo_surface& fill(cairo_image_surface const& surface, pixel_store::vec4_t<int> location)
		{
			auto const dim = surface.get_dimensions();
			cairo_set_source_surface(m_ctxt.get(), surface.handle(), 0, 0);
			cairo_rectangle(m_ctxt.get(), location[0], location[1], dim.width, dim.height);
			cairo_set_operator(m_ctxt.get(), CAIRO_OPERATOR_OVER);
			cairo_fill(m_ctxt.get());
			return *this;
		}

		cairo_surface& set_dimensions(dimensions dim)
		{
			cairo_xlib_surface_set_size(m_surface.get(), dim.width, dim.height);
			m_dim = dim;
			return *this;
		}

		dimensions get_dimensions() const { return m_dim; }

	private:
		surface_type m_surface;
		ctxt_type m_ctxt;
		dimensions m_dim;
	};
}

#endif
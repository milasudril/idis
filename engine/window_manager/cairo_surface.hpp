//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./cairo_surface.o", "rel":"implementation"},
//@		{"ref":"cairo", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_WINDOWMANAGER_CAIROSURFACE_HPP
#define IDIS_WINDOWMANAGER_CAIROSURFACE_HPP

#include "./window.hpp"

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

	class cairo_surface
	{
		using ctxt_type    = std::unique_ptr<cairo_t, detail::cairo_deleter>;
		using surface_type = std::unique_ptr<cairo_surface_t, detail::cairo_deleter>;

	public:
		explicit cairo_surface(window& target_window);

	private:
		surface_type m_surface;
		ctxt_type m_ctxt;
	};
}

#endif
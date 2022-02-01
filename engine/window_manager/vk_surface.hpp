//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./vk_surface.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_WINDOWMANAGER_VKSURFACE_HPP
#define IDIS_WINDOWMANAGER_VKSURFACE_HPP

#include "./window.hpp"
#include "./vk_instance.hpp"

#include <type_traits>
#include <cassert>

namespace idis::wm
{
	namespace detail
	{
		struct vk_surface_deleter
		{
			void operator()(VkSurfaceKHR obj) const
			{
				assert(instance != nullptr);
				vkDestroySurfaceKHR(instance, obj, nullptr);
			}

			VkInstance instance{};
		};
	}

	class vk_surface
	{
	public:
		using handle_type =
		    std::unique_ptr<std::remove_pointer_t<VkSurfaceKHR>, detail::vk_surface_deleter>;

		vk_surface() = default;

		explicit vk_surface(vk_instance& instance, window_base& target_window);

		VkSurfaceKHR handle() const { return m_handle.get(); }

	private:
		handle_type m_handle;
	};


	//  vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
	/*	vk_queue_family_info const* find_surface_queue(VkPhysicalDevice,
												std::span<vk_queue_family_info const> queues,
												vk_surface const& surface)*/
}

#endif
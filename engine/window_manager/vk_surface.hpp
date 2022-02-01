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
				if(obj != nullptr)
				{
					assert(instance != nullptr);
					vkDestroySurfaceKHR(instance, obj, nullptr);
				}
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

	struct vk_render_device
	{
		VkPhysicalDevice device;
		int device_index;
		int graphics_queue_family;
		int surface_queue_family;
	};

	std::string to_string(vk_render_device const& device);

	/**
	 * \note queue families must be sorted by the device they belong to.
	 */
	std::vector<vk_render_device> collect_usable_devices(
	    std::span<vk_queue_family_info const> queues, vk_surface const& surface);
}

#endif
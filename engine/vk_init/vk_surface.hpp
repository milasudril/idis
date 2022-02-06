//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./vk_surface.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_WINDOWMANAGER_VKSURFACE_HPP
#define IDIS_WINDOWMANAGER_VKSURFACE_HPP

#include "./vk_instance.hpp"

#include "engine/window_manager/window.hpp"

#include <type_traits>

namespace idis::wm
{
	namespace detail
	{
		class vk_surface_deleter
		{
		public:
			explicit vk_surface_deleter(VkInstance instance): m_instance{instance} {}

			void operator()(VkSurfaceKHR obj) const
			{
				if(obj != nullptr) { vkDestroySurfaceKHR(m_instance, obj, nullptr); }
			}

		private:
			VkInstance m_instance;
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

	std::vector<vk_render_device> collect_usable_devices(
	    vk_system_info::queue_family_list_view queue_families, vk_surface const& surface);


	inline auto get_surface_capabilities(VkPhysicalDevice device, vk_surface const& surface)
	{
		VkSurfaceCapabilitiesKHR ret{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface.handle(), &ret);
		return ret;
	}

	inline auto select_extent(VkSurfaceCapabilitiesKHR const& caps, window_base const& window)
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

	inline auto get_image_count(VkSurfaceCapabilitiesKHR const& caps)
	{
		auto img_count = caps.minImageCount + 2;
		if(caps.maxImageCount > 0) { return std::min(img_count, caps.maxImageCount); }
		return img_count;
	}

	inline auto get_surface_formats(VkPhysicalDevice device, vk_surface const& surface)
	{
		uint32_t format_count{};
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface.handle(), &format_count, nullptr);
		std::vector<VkSurfaceFormatKHR> ret(format_count);
		if(format_count != 0)
		{
			vkGetPhysicalDeviceSurfaceFormatsKHR(
			    device, surface.handle(), &format_count, std::data(ret));
		}
		return ret;
	}

	inline auto get_surface_present_modes(VkPhysicalDevice device, vk_surface const& surface)
	{
		uint32_t count{};
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface.handle(), &count, nullptr);
		std::vector<VkPresentModeKHR> ret(count);
		if(count != 0)
		{
			vkGetPhysicalDeviceSurfacePresentModesKHR(
			    device, surface.handle(), &count, std::data(ret));
		}
		return ret;
	}
}

#endif
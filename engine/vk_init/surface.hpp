//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./surface.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_VKINIT_SURFACE_HPP
#define IDIS_VKINIT_SURFACE_HPP

#include "./instance.hpp"

#include "engine/window_manager/window.hpp"

#include <type_traits>

namespace idis::vk_init
{
	class surface_deleter
	{
	public:
		explicit surface_deleter(VkInstance assoc_instance): m_instance{assoc_instance} {}

		void operator()(VkSurfaceKHR obj) const
		{
			if(obj != nullptr) { vkDestroySurfaceKHR(m_instance, obj, nullptr); }
		}

	private:
		VkInstance m_instance;
	};

	class surface
	{
	public:
		using handle_type = std::unique_ptr<std::remove_pointer_t<VkSurfaceKHR>, surface_deleter>;

		surface() = default;

		explicit surface(instance& assoc_instance, wm::window_base& target_window);

		VkSurfaceKHR handle() const { return m_handle.get(); }

	private:
		handle_type m_handle;
	};

	struct render_device
	{
		VkPhysicalDevice device;
		int device_index;
		int graphics_queue_family;
		int surface_queue_family;
	};

	std::string to_string(render_device const& device);

	std::vector<render_device> collect_usable_devices(system::queue_family_list_view queue_families,
	                                                  surface const& surface);


	inline auto get_surface_capabilities(VkPhysicalDevice device, surface const& surface)
	{
		VkSurfaceCapabilitiesKHR ret{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface.handle(), &ret);
		return ret;
	}

	inline auto get_image_count(VkSurfaceCapabilitiesKHR const& caps)
	{
		auto img_count = caps.minImageCount + 2;
		if(caps.maxImageCount > 0) { return std::min(img_count, caps.maxImageCount); }
		return img_count;
	}

	inline auto get_surface_formats(VkPhysicalDevice device, surface const& surface)
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

	inline auto get_surface_present_modes(VkPhysicalDevice device, surface const& surface)
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

	render_device select_device(std::string_view prefered_device,
	                            system const& sysinfo,
	                            surface const& surf);

	VkPresentModeKHR select_present_mode(VkPhysicalDevice device, surface const& surf);


	VkSurfaceFormatKHR select_surface_format(VkPhysicalDevice device, surface const& surf);
}

#endif
//@	{
//@	 "target":{"name":"vk_surface.o", "pkgconfig_libs":["vulkan"]}
//@	}

#include "./vk_surface.hpp"

#include <algorithm>

namespace
{
	auto create_vk_surface(idis::wm::vk_instance& instance, idis::wm::window_base& window)
	{
		VkSurfaceKHR surface{};
		if(glfwCreateWindowSurface(instance.handle(), window.handle(), nullptr, &surface) != VK_SUCCESS)
		{
			throw idis::exception{"create vulkan surface", ""};
		}

		return idis::wm::vk_surface::handle_type{surface, idis::wm::detail::vk_surface_deleter{instance.handle()}};
	}
}


idis::wm::vk_surface::vk_surface(vk_instance& instance, window_base& window):m_handle{create_vk_surface(instance, window)}
{
}

std::vector<idis::wm::vk_render_device> idis::wm::collect_usable_devices(
    vk_system_info::queue_family_list_view queue_families, vk_surface const& surface)
{
	std::vector<idis::wm::vk_render_device> ret{};
	if(std::size(queue_families) == 0) { return ret; }

	auto const surf = surface.handle();

	auto ptr                   = std::begin(queue_families);
	auto dev_current           = ptr->device;
	auto graphics_queue_family = -1;
	auto surface_queue_family  = -1;
	while(ptr != std::end(queue_families))
	{
		if(ptr->device != dev_current)
		{
			if(graphics_queue_family != -1 && surface_queue_family != -1)
			{
				ret.push_back(vk_render_device{dev_current,
				                               ptr->device_index - 1,
				                               graphics_queue_family,
				                               surface_queue_family});
			}

			dev_current           = ptr->device;
			graphics_queue_family = -1;
			surface_queue_family  = -1;
		}

		if(graphics_queue_family == -1)
		{
			if(ptr->properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphics_queue_family = ptr->family_index;
			}
		}

		if(surface_queue_family == -1)
		{
			VkBool32 has_surface_support{};
			vkGetPhysicalDeviceSurfaceSupportKHR(
			    ptr->device, ptr->family_index, surf, &has_surface_support);
			if(has_surface_support) { surface_queue_family = ptr->family_index; }
		}
		++ptr;
	}

	return ret;
}

std::string idis::wm::to_string(vk_render_device const& device)
{
	return std::string{"device="}
	    .append(std::to_string(reinterpret_cast<intptr_t>(device.device)))
	    .append("; device_index=")
	    .append(std::to_string(device.device_index))
	    .append("; graphics_queue_family=")
	    .append(std::to_string(device.graphics_queue_family))
	    .append("; surface_queue_family=")
	    .append(std::to_string(device.surface_queue_family));
}

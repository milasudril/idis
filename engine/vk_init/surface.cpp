//@	{
//@	 "target":{"name":"surface.o", "pkgconfig_libs":["vulkan"]}
//@	}

#include "./surface.hpp"

#include <algorithm>

namespace
{
	auto create_surface(idis::vk_init::instance& instance, idis::wm::window_base& window)
	{
		VkSurfaceKHR surface{};
		if(glfwCreateWindowSurface(instance.handle(), window.handle(), nullptr, &surface)
		   != VK_SUCCESS)
		{
			throw idis::exception{"create vulkan surface", ""};
		}

		return idis::vk_init::surface::handle_type{
		    surface, idis::vk_init::surface_deleter{instance.handle()}};
	}
}


idis::vk_init::surface::surface(instance& assoc_instance, wm::window_base& window)
    : m_handle{create_surface(assoc_instance, window)}
{
}

std::vector<idis::vk_init::render_device> idis::vk_init::collect_usable_devices(
    system::queue_family_list_view queue_families, surface const& surface)
{
	std::vector<render_device> ret{};
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
				ret.push_back(render_device{dev_current,
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

std::string idis::vk_init::to_string(render_device const& device)
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

namespace
{
	size_t rank(VkPhysicalDeviceType value)
	{
		switch(value)
		{
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return 0;
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return 1;
			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: return 1;
			case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			default: return static_cast<size_t>(-1);
		}
	}
}

idis::vk_init::render_device idis::vk_init::select_device(std::string_view prefered_device,
                                                          system const& sysinfo,
                                                          surface const& surf)
{
	auto queue_families = sysinfo.queue_families();
	auto usable_devices = collect_usable_devices(queue_families, surf);

	if(std::size(usable_devices) == 0)
	{
		throw exception{"select graphics devices", "No usable graphics device found"};
	}

	auto devices = sysinfo.devices();

	if(auto i = std::ranges::find_if(usable_devices,
	                                 [devices, prefered_device](auto const& item) {
		                                 return devices[item.device_index].properties.deviceName
		                                        == prefered_device;
	                                 });
	   i != std::end(usable_devices))
	{
		return *i;
	}

	std::ranges::sort(usable_devices,
	                  [devices](auto const& a, auto const& b)
	                  {
		                  auto const type_a = devices[a.device_index].properties.deviceType;
		                  auto const type_b = devices[b.device_index].properties.deviceType;
		                  return rank(type_a) < rank(type_b);
	                  });

	return usable_devices[0];
}
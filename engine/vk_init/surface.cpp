//@	{
//@	 "target":{"name":"surface.o", "pkgconfig_libs":["vulkan"]}
//@	}

#include "./surface.hpp"

#include <algorithm>

namespace
{
	auto create_surface(std::reference_wrapper<idis::vk_init::instance const> instance,
	                    std::reference_wrapper<idis::wm::window_base const> window)
	{
		VkSurfaceKHR surface{};
		if(auto res = glfwCreateWindowSurface(
		       instance.get().handle(), window.get().handle(), nullptr, &surface); res != VK_SUCCESS)
		{
			throw idis::exception{"create vulkan surface", to_string(idis::vk_init::error{res})};
		}

		return idis::vk_init::surface::handle_type{
		    surface, idis::vk_init::surface_deleter{instance.get().handle()}};
	}
}


idis::vk_init::surface::surface(std::reference_wrapper<instance const> assoc_instance,
                                std::reference_wrapper<wm::window_base const> window)
    : m_handle{create_surface(assoc_instance, window)}
    , m_target_window{window}
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

namespace
{
	size_t rank(VkPresentModeKHR value)
	{
		switch(value)
		{
			case VK_PRESENT_MODE_MAILBOX_KHR: return 0;
			case VK_PRESENT_MODE_IMMEDIATE_KHR: return 1;
			default: return static_cast<size_t>(-1);
		}
	}
}

VkPresentModeKHR idis::vk_init::select_present_mode(VkPhysicalDevice device, surface const& surf)
{
	auto present_modes = get_surface_present_modes(device, surf);
	std::ranges::sort(present_modes, [](auto a, auto b) { return rank(a) < rank(b); });

	auto const present_mode = present_modes[0];
	if(!(present_mode == VK_PRESENT_MODE_MAILBOX_KHR
	     || present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR))
	{
		throw exception{"configure surface", "No suitable present mode found"};
	}
	return present_mode;
}

VkSurfaceFormatKHR idis::vk_init::select_surface_format(VkPhysicalDevice device,
                                                        surface const& surf)
{
	auto const surface_formats = get_surface_formats(device, surf);

	auto const surface_format =
	    std::ranges::find_if(surface_formats,
	                         [](auto const& item)
	                         {
		                         return item.format == VK_FORMAT_B8G8R8A8_SRGB
		                                && item.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	                         });

	if(surface_format == std::end(surface_formats))
	{
		throw exception{"configure surface", "No suitable surface format found"};
	}

	return *surface_format;
}
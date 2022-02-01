//@	{
//@	 "target":{"name":"vk_surface.o", "pkgconfig_libs":["vulkan"]}
//@	}

#include "./vk_surface.hpp"

#include <vector>

idis::wm::vk_surface::vk_surface(vk_instance& instance, window_base& window)
{
	VkSurfaceKHR surface{};
	if(glfwCreateWindowSurface(instance.handle(), window.handle(), nullptr, &surface) != VK_SUCCESS)
	{
		throw exception{"create vulkan surface", ""};
	}
	m_handle = handle_type{surface, detail::vk_surface_deleter{instance.handle()}};
}

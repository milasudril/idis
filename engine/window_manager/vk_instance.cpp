//@{"target":{"name":"vk_instance.o"}}

#include "./vk_instance.hpp"

#include "engine/error_handler/exception.hpp"

idis::wm::vk_instance::vk_instance()
{
	if(glfwVulkanSupported() == GLFW_FALSE)
	{
		throw exception{"create vulkan surface", "No driver is available"};
	}

	VkInstance instance{};
	VkApplicationInfo app_info{};
	app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName   = "idis";
	app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	app_info.pEngineName        = "idis";
	app_info.engineVersion      = VK_MAKE_VERSION(0, 0, 1);
	app_info.apiVersion         = VK_API_VERSION_1_2;

	VkInstanceCreateInfo create_info{};
	create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;
	create_info.ppEnabledExtensionNames =
	    glfwGetRequiredInstanceExtensions(&create_info.enabledExtensionCount);
	if(create_info.ppEnabledExtensionNames == nullptr)
	{
		throw exception{"create vulkan surface",
		                "The selected driver has no drawing capabilities or is not available"};
	}

	if(vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
	{
		throw exception{"create vulkan surface", "No instance created"};
	}

	m_handle.reset(instance);
}

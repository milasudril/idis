//@	{
//@	 "target":{"name":"vulkan_surface.o", "pkgconfig_libs":["vulkan"]}
//@	}

#include "./vulkan_surface.hpp"

#include <vector>

idis::wm::vulkan_surface::vulkan_surface(window_base& window)
{
	if(glfwVulkanSupported() == GLFW_FALSE)
	{
		throw exception{"create vulkan surface", "No driver is available"};
	}

	{
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

		m_instance.reset(instance);
	}

#if 1
	uint32_t dev_count{};
	vkEnumeratePhysicalDevices(m_instance.get(), &dev_count, nullptr);
	if(dev_count == 0) { throw exception{"create vulkan surface", "No physical device"}; }

	std::vector<VkPhysicalDevice> devices(dev_count);
	vkEnumeratePhysicalDevices(m_instance.get(), &dev_count, devices.data());
	for(size_t k = 0; k != std::size(devices); ++k)
	{
		VkPhysicalDeviceProperties props{};
		vkGetPhysicalDeviceProperties(devices[k], &props);
		VkPhysicalDeviceFeatures features{};
		vkGetPhysicalDeviceFeatures(devices[k], &features);

		printf("name=%s, type=%d, id=%d, api_version=%d.%d.%d\n",
		       props.deviceName,
		       props.deviceType,
		       props.deviceID,
		       VK_VERSION_MAJOR(props.apiVersion),
		       VK_VERSION_MINOR(props.apiVersion),
		       VK_VERSION_PATCH(props.apiVersion));

		uint32_t qf_count{};
		vkGetPhysicalDeviceQueueFamilyProperties(devices[k], &qf_count, nullptr);
		std::vector<VkQueueFamilyProperties> qfs(qf_count);
		vkGetPhysicalDeviceQueueFamilyProperties(devices[k], &qf_count, qfs.data());
		for(size_t l = 0; l != std::size(qfs); ++l)
		{
			printf("   flags=%08x, num_queus=%u\n", qfs[l].queueFlags, qfs[l].queueCount);
		}
	}

	{
		// FIXME: Choose device depending on capabilities and type. On my setup, device 0
		//        and queue 0 is correct.
		auto const selected_dev          = 0;
		auto const selected_queue_family = 0;

		// FIXME: Need two queues
		// NOTE: graphics_queue and presentation_queue can be in different families
		VkDeviceQueueCreateInfo queue_create_info{};
		queue_create_info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex = selected_queue_family;
		queue_create_info.queueCount       = 1;
		auto const queue_prio              = 1.0f;
		queue_create_info.pQueuePriorities = &queue_prio;

		VkPhysicalDeviceFeatures features{};

		VkDeviceCreateInfo create_info{};
		create_info.pQueueCreateInfos    = &queue_create_info;
		create_info.queueCreateInfoCount = 1;
		create_info.pEnabledFeatures     = &features;

		VkDevice device{};
		if(vkCreateDevice(devices[selected_dev], &create_info, nullptr, &device) != VK_SUCCESS)
		{
			throw exception{"create vulkan surface", "Failed to create logical device"};
		}

		m_device.reset(device);
	}

	VkQueue graphics_queue;
	vkGetDeviceQueue(m_device.get(), 0 /*should use correct queue here*/, 0, &graphics_queue);
#endif

	// FIXME: We need to setup the surface before selecting device, so we know that we have presentation
	//        capabilities on current surface.
	VkSurfaceKHR surface{};
	if(glfwCreateWindowSurface(m_instance.get(), window.handle(), nullptr, &surface) != VK_SUCCESS)
	{
		throw exception{"create vulkan surface", ""};
	}

	VkBool32 present_support{false};
	vkGetPhysicalDeviceSurfaceSupportKHR(
	    devices[0], 0 /*should use correct queue here*/, surface, &present_support);
	if(!present_support)
	{
		vkDestroySurfaceKHR(m_instance.get(), surface, nullptr);
		throw exception{"create vulkan surface", "The selected queue has no presentation support"};
	}

	vkDestroySurfaceKHR(m_instance.get(), surface, nullptr);
}

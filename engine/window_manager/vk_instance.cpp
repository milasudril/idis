//@{"target":{"name":"vk_instance.o"}}

#include "./vk_instance.hpp"

#include "engine/error_handler/exception.hpp"

#include <algorithm>

namespace
{
	idis::wm::vk_instance::handle_type make_vk_instance()
	{
		if(glfwVulkanSupported() == GLFW_FALSE)
		{
			throw idis::exception{"create vulkan instance", "No driver is available"};
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
			throw idis::exception{
			    "create vulkan instance",
			    "The selected driver has no drawing capabilities or is not available"};
		}

		if(vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
		{
			throw idis::exception{"create vulkan instance", "Operation failed"};
		}
		return idis::wm::vk_instance::handle_type{instance};
	}

	template<class T>
	std::string get_api_version_string(T api_version)
	{
		return std::to_string(VK_VERSION_MAJOR(api_version))
		    .append(".")
		    .append(std::to_string(VK_VERSION_MINOR(api_version)))
		    .append(".")
		    .append(std::to_string(VK_VERSION_PATCH(api_version)));
	}

	std::string get_device_uuid_string(uint8_t const (&bytes)[VK_UUID_SIZE])
	{
		std::string ret{};
		for(size_t k = 0; k != VK_UUID_SIZE; ++k)
		{
			auto const msb    = static_cast<char>((bytes[k] & 0xf0) >> 4);
			auto const lsb    = static_cast<char>(bytes[k] & 0x0f);
			auto const ch_msb = msb < 10 ? (msb + '0') : ((msb - 10) + 'a');
			auto const ch_lsb = lsb < 10 ? (lsb + '0') : ((lsb - 10) + 'a');
			ret += ch_msb;
			ret += ch_lsb;
		}
		return ret;
	}
}

std::string idis::wm::to_string(vk_device_info const& dev_info)
{
	auto const& props = dev_info.properties;

	return std::string{"name="}
	    .append(props.deviceName)
	    .append("; vendor_id=")
	    .append(std::to_string(props.deviceID))
	    .append("; uuid=")
	    .append(get_device_uuid_string(props.pipelineCacheUUID))
	    .append("; driver_version=")
	    .append(std::to_string(props.driverVersion))
	    .append("; type=")
	    .append(std::to_string(props.deviceType))
	    .append("; api_version=")
	    .append(get_api_version_string(props.apiVersion));
}

std::string idis::wm::to_string(vk_queue_family_info const& family_info)
{
	return std::string{"device="}
	    .append(std::to_string(reinterpret_cast<intptr_t>(family_info.device)))
	    .append("; device_index=")
	    .append(std::to_string(family_info.device_index))
	    .append("; family_index=")
	    .append(std::to_string(family_info.family_index))
	    .append("; queue_flags=")
	    .append(std::to_string(family_info.properties.queueFlags))
	    .append("; queue_count=")
	    .append(std::to_string(family_info.properties.queueCount));
}

idis::wm::vk_system_info::vk_system_info(VkInstance instance)
{
	uint32_t dev_count{};
	vkEnumeratePhysicalDevices(instance, &dev_count, nullptr);
	if(dev_count == 0)
	{
		throw exception{"collect information about Vulkan devices", "No physical device"};
	}

	std::vector<VkPhysicalDevice> devices(dev_count);
	vkEnumeratePhysicalDevices(instance, &dev_count, std::data(devices));
	std::ranges::transform(devices,
	                       std::back_inserter(m_devices),
	                       [](auto device)
	                       {
		                       vk_device_info dev_info{};
		                       dev_info.device = device;
		                       vkGetPhysicalDeviceProperties(device, &dev_info.properties);
		                       return dev_info;
	                       });

	std::vector<vk_queue_family_info> qf;
	std::ranges::for_each(
	    devices,
	    [&queue_families = qf, device_index = 0](auto device) mutable
	    {
		    uint32_t qf_count{};
		    vkGetPhysicalDeviceQueueFamilyProperties(device, &qf_count, nullptr);
		    std::vector<VkQueueFamilyProperties> qfs(qf_count);
		    vkGetPhysicalDeviceQueueFamilyProperties(device, &qf_count, std::data(qfs));
		    std::ranges::transform(
		        qfs,
		        std::back_inserter(queue_families),
		        [dev          = std::as_const(device),
		         dev_index    = std::as_const(device_index),
		         family_index = 0](auto const& props) mutable
		        {
			        auto ret = vk_queue_family_info{dev, dev_index, family_index, props};
			        ++family_index;
			        return ret;
		        });
		    ++device_index;
	    });
	m_queue_families = queue_family_list{std::move(qf)};
}

idis::wm::vk_instance::vk_instance(): m_handle{make_vk_instance()}, m_system_info{m_handle.get()} {}

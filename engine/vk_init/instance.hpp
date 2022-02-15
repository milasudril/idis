//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./instance.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_VKINIT_INSTANCE_HPP
#define IDIS_VKINIT_INSTANCE_HPP

#include "engine/window_managment/initializer.hpp"
#include "engine/utils/sorted_sequence.hpp"

#include <memory>
#include <vector>
#include <string>
#include <span>

namespace idis::vk_init
{
	struct device_info
	{
		VkPhysicalDevice device;
		VkPhysicalDeviceProperties properties;
	};

	std::string to_string(device_info const&);

	struct queue_family_info
	{
		VkPhysicalDevice device;
		int device_index;
		int family_index;
		VkQueueFamilyProperties properties;

		struct cmp_dev_index
		{
			bool operator()(queue_family_info const& a, queue_family_info const& b) const
			{
				return a.device_index < b.device_index;
			}
		};
	};


	std::string to_string(queue_family_info const&);

	class system
	{
	public:
		using queue_family_list = sorted_sequence<queue_family_info,
		                                          varlength_array<queue_family_info>,
		                                          queue_family_info::cmp_dev_index>;

		using queue_family_list_view =
		    sorted_view<queue_family_info, queue_family_info::cmp_dev_index>;

		explicit system(VkInstance instance);

		std::span<device_info const> devices() const { return m_devices; }

		auto queue_families() const { return queue_family_list_view{std::ref(m_queue_families)}; }

	private:
		std::vector<device_info> m_devices;
		queue_family_list m_queue_families;
	};

	struct instance_deleter
	{
		void operator()(VkInstance inst) const
		{
			if(inst != nullptr) { vkDestroyInstance(inst, nullptr); }
		}
	};

	class instance
	{
	public:
		using handle_type = std::unique_ptr<std::remove_pointer_t<VkInstance>, instance_deleter>;

		instance();

		auto handle() const { return m_handle.get(); }

		auto const& system_info() const { return m_system_info; }

	private:
		[[no_unique_address]] wm::initializer m_init;
		handle_type m_handle;
		system m_system_info;
	};
}

#endif

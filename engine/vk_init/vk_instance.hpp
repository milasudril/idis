//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./vk_instance.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_VKINIT_INSTANCE_HPP
#define IDIS_VKINIT_INSTANCE_HPP

#include "engine/window_manager/initializer.hpp"
#include "engine/helpers/sorted_sequence.hpp"

#include <memory>
#include <vector>
#include <string>
#include <span>

namespace idis::wm
{
	struct vk_device_info
	{
		VkPhysicalDevice device;
		VkPhysicalDeviceProperties properties;
	};

	std::string to_string(vk_device_info const&);

	struct vk_queue_family_info
	{
		VkPhysicalDevice device;
		int device_index;
		int family_index;
		VkQueueFamilyProperties properties;

		struct cmp_dev_index
		{
			bool operator()(vk_queue_family_info const& a, vk_queue_family_info const& b) const
			{
				return a.device_index < b.device_index;
			}
		};
	};


	std::string to_string(vk_queue_family_info const&);

	class vk_system_info
	{
	public:
		using queue_family_list = sorted_sequence<vk_queue_family_info,
		                                          std::vector<vk_queue_family_info>,
		                                          vk_queue_family_info::cmp_dev_index>;

		using queue_family_list_view =
		    sorted_view<vk_queue_family_info, vk_queue_family_info::cmp_dev_index>;

		explicit vk_system_info(VkInstance instance);

		std::span<vk_device_info const> devices() const { return m_devices; }

		auto queue_families() const { return queue_family_list_view{std::ref(m_queue_families)}; }

	private:
		std::vector<vk_device_info> m_devices;
		queue_family_list m_queue_families;
	};

	namespace detail
	{
		struct vk_instance_deleter
		{
			void operator()(VkInstance inst) const
			{
				if(inst != nullptr) { vkDestroyInstance(inst, nullptr); }
			}
		};
	}

	class vk_instance
	{
	public:
		using handle_type =
		    std::unique_ptr<std::remove_pointer_t<VkInstance>, detail::vk_instance_deleter>;

		vk_instance();

		auto handle() const { return m_handle.get(); }

		auto const& system_info() const { return m_system_info; }

	private:
		[[no_unique_address]] initializer m_init;
		handle_type m_handle;
		vk_system_info m_system_info;
	};
}

#endif

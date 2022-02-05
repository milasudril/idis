//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./vk_device.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_WINDOWMANAGER_VKDEVICE_HPP
#define IDIS_WINDOWMANAGER_VKDEVICE_HPP

#include "./vk_surface.hpp"

namespace idis::wm
{
	namespace detail
	{
		struct vk_device_deleter
		{
			void operator()(VkDevice dev)
			{
				if(dev != nullptr) { vkDestroyDevice(dev, nullptr); }
			}
		};
	}

	class vk_device
	{
	public:
		using handle_type =
		    std::unique_ptr<std::remove_pointer_t<VkDevice>, detail::vk_device_deleter>;

		explicit vk_device(vk_render_device const& device);

		VkDevice handle() const { return m_handle.get(); }

		VkQueue get_graphics_queue() const
		{
			VkQueue queue{};
			vkGetDeviceQueue(m_handle.get(), m_graphics_queue, 0, &queue);
			return queue;
		}

		VkQueue get_surface_queue() const
		{
			VkQueue queue{};
			vkGetDeviceQueue(m_handle.get(), m_surface_queue, 0, &queue);
			return queue;
		}

		auto get_graphics_queue_family() const { return m_graphics_queue; }

		auto get_surface_queue_family() const { return m_surface_queue; }

	private:
		handle_type m_handle;
		int m_graphics_queue;
		int m_surface_queue;
	};
}

#endif
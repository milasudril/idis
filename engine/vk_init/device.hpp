//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./device.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_VKINIT_DEVICE_HPP
#define IDIS_VKINIT_DEVICE_HPP

#include "./surface.hpp"

namespace idis::vk_init
{
	struct device_deleter
	{
		void operator()(VkDevice dev)
		{
			if(dev != nullptr) { vkDestroyDevice(dev, nullptr); }
		}
	};

	class device
	{
	public:
		using handle_type = std::unique_ptr<std::remove_pointer_t<VkDevice>, device_deleter>;

		explicit device(render_device const& device_info);

		VkDevice handle() const { return m_handle.get(); }

		VkQueue graphics_queue() const
		{
			VkQueue queue{};
			vkGetDeviceQueue(m_handle.get(), m_graphics_queue, 0, &queue);
			return queue;
		}

		VkQueue surface_queue() const
		{
			VkQueue queue{};
			vkGetDeviceQueue(m_handle.get(), m_surface_queue, 0, &queue);
			return queue;
		}

		auto graphics_queue_family() const { return m_graphics_queue; }

		auto surface_queue_family() const { return m_surface_queue; }

		render_device const& device_info() const { return m_device_info; }

		void wait() const
		{
			vkDeviceWaitIdle(m_handle.get());
		}

	private:
		handle_type m_handle;
		int m_graphics_queue;
		int m_surface_queue;
		render_device m_device_info;
	};
}

#endif
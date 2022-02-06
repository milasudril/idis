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

		explicit device(render_device const& device);

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
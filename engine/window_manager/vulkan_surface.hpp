//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./vulkan_surface.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_WINDOWMANAGER_VULKANSURFACE_HPP
#define IDIS_WINDOWMANAGER_VULKANSURFACE_HPP

#include "./window.hpp"

#include <type_traits>

namespace idis::wm
{
	namespace detail
	{
		struct vulkan_deleter
		{
			void operator()(VkInstance ctxt) const
			{
				if(ctxt != nullptr) { vkDestroyInstance(ctxt, nullptr); }
			}

			void operator()(VkDevice dev) const
			{
				if(dev != nullptr) { vkDestroyDevice(dev, nullptr); }
			}
		};
	}

	class vulkan_surface
	{
	public:
		vulkan_surface() = default;

		explicit vulkan_surface(window_base& target_window);

	private:
		std::unique_ptr<std::remove_pointer_t<VkInstance>, detail::vulkan_deleter> m_instance;
		std::unique_ptr<std::remove_pointer_t<VkDevice>, detail::vulkan_deleter> m_device;
	};
}

#endif
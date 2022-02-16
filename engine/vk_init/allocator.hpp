//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./allocator.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_VKINIT_ALLOCATOR_HPP
#define IDIS_VKINIT_ALLOCATOR_HPP

#include "./device.hpp"
#include "./instance.hpp"

#include "vma/vk_mem_alloc.h"

namespace idis::vk_init
{
	struct allocator_deleter
	{
		void operator()(VmaAllocator obj)
		{
			if(obj != nullptr) { vmaDestroyAllocator(obj); }
		}
	};

	using allocator_handle =
	    std::unique_ptr<std::remove_pointer_t<VmaAllocator>, allocator_deleter>;

	inline auto create_allocator(std::reference_wrapper<device const> dev, VkInstance instance)
	{
		VmaAllocatorCreateInfo create_info{};
		create_info.vulkanApiVersion = VK_API_VERSION_1_2;
		create_info.physicalDevice   = dev.get().device_info().device;
		create_info.device           = dev.get().handle();
		create_info.instance         = instance;

		VmaAllocator ret{};
		vmaCreateAllocator(&create_info, &ret);
		return allocator_handle{ret};
	}

	class allocator
	{
	public:
		using handle_type = allocator_handle;

		explicit allocator(std::reference_wrapper<device const> dev, VkInstance instance)
		    : m_handle{create_allocator(dev, instance)}
		{
		}

		VmaAllocator handle() const { return m_handle.get(); }

	private:
		handle_type m_handle;
	};
}

#endif
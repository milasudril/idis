//@	{
//@	 "dependencies_extra": [{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_BUFFER_HPP
#define IDIS_GPURES_BUFFER_HPP

#include "engine/vk_init/allocator.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class buffer_deleter
	{
	public:
		explicit buffer_deleter(VmaAllocator allocator, VmaAllocation allocation)
		    : m_allocator{allocator}
		    , m_allocation{allocation}
		{
		}

		void operator()(VkBuffer obj) const
		{
			if(obj != nullptr) { vmaDestroyBuffer(m_allocator, obj, m_allocation); }
		}

		VmaAllocation allocation() const { return m_allocation; }

	private:
		VmaAllocator m_allocator;
		VmaAllocation m_allocation;
	};

	using buffer_handle = std::unique_ptr<std::remove_pointer_t<VkBuffer>, buffer_deleter>;

	template<auto BufferUsage, auto AllocationUsage = VMA_MEMORY_USAGE_CPU_TO_GPU>
	auto create_buffer(VmaAllocator allocator, size_t capacity)
	{
		VkBufferCreateInfo buffer_info{};
		buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_info.size  = capacity;
		buffer_info.usage = BufferUsage;

		VmaAllocationCreateInfo alloc_info{};
		alloc_info.usage = AllocationUsage;

		VkBuffer buffer{};
		VmaAllocation allocation{};
		if(auto res =
		       vmaCreateBuffer(allocator, &buffer_info, &alloc_info, &buffer, &allocation, nullptr);
		   res != VK_SUCCESS)
		{
			throw idis::exception{"create buffer", to_string(idis::vk_init::error{res})};
		}

		return buffer_handle{buffer, buffer_deleter{allocator, allocation}};
	}

	template<auto BufferUsage, auto AllocationUsage = VMA_MEMORY_USAGE_CPU_TO_GPU>
	class buffer
	{
	public:
		using handle_type                      = buffer_handle;
		static constexpr auto buffer_usage     = BufferUsage;
		static constexpr auto allocation_usage = AllocationUsage;

		explicit buffer(std::reference_wrapper<vk_init::allocator const> allocator, size_t size)
		    : buffer{allocator.get().handle(), size}
		{
		}

		explicit buffer(VmaAllocator allocator, size_t size)
		    : m_handle{create_buffer<BufferUsage, AllocationUsage>(allocator, size)}
		{
		}

		VkBuffer handle() const { return m_handle.get(); }

	private:
		handle_type m_handle;
	};

	template<auto AllocationUsage = VMA_MEMORY_USAGE_CPU_TO_GPU>
	auto create_vertex_buffer(std::reference_wrapper<vk_init::allocator const> allocator,
	                          size_t size)
	{
		return buffer<VK_BUFFER_USAGE_VERTEX_BUFFER_BIT>{allocator, size};
	}
}

#endif
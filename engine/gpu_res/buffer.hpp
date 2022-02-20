//@	{
//@	 "dependencies_extra": [{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_BUFFER_HPP
#define IDIS_GPURES_BUFFER_HPP

#include "engine/vk_init/allocator.hpp"

#include <type_traits>
#include <memory>
#include <cstring>
#include <cassert>

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

		VmaAllocator allocator() const { return m_allocator; }

	private:
		VmaAllocator m_allocator;
		VmaAllocation m_allocation;
	};

	using buffer_handle = std::unique_ptr<std::remove_pointer_t<VkBuffer>, buffer_deleter>;

	template<auto BufferUsage, auto AllocationFlags>
	auto create_buffer(VmaAllocator allocator, size_t capacity)
	{
		VkBufferCreateInfo buffer_info{};
		buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_info.size  = capacity;
		buffer_info.usage = BufferUsage;

		VmaAllocationCreateInfo alloc_info{};
		alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
		alloc_info.flags = AllocationFlags;

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

	template<auto BufferUsage, auto AllocationFlags>
	class buffer
	{
	public:
		using handle_type                      = buffer_handle;
		static constexpr auto buffer_usage     = BufferUsage;
		static constexpr auto allocation_flags = AllocationFlags;

		explicit buffer(std::reference_wrapper<vk_init::allocator const> allocator, size_t size)
		    : buffer{allocator.get().handle(), size}
		{
		}

		explicit buffer(VmaAllocator allocator, size_t size)
		    : m_handle{create_buffer<BufferUsage, AllocationFlags>(allocator, size)}
		{
		}

		VkBuffer handle() const { return m_handle.get(); }

		VmaAllocator allocator() const { return m_handle.get_deleter().allocator(); }

		VmaAllocation allocation() const { return m_handle.get_deleter().allocation(); }

		size_t capacity() const
		{
			VmaAllocationInfo alloc_info{};
			vmaGetAllocationInfo(allocator(), allocation(), &alloc_info);
			return alloc_info.size;
		}

	private:
		handle_type m_handle;
	};

	template<auto AllocationFlags>
	using vertex_buffer = buffer<VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, AllocationFlags>;

	constexpr auto is_host_visible(VmaAllocationCreateFlagBits allocation_flags)
	{
		return static_cast<bool>(allocation_flags
		                         & VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT)
		       || static_cast<bool>(allocation_flags
		                            & VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT);
	}

	template<auto BufferUsage, auto AllocationFlags, class SrcType>
	requires(std::is_trivially_copyable_v<SrcType>&& is_host_visible(AllocationFlags)) size_t
	    sync_transfer(buffer<BufferUsage, AllocationFlags> const& buffer,
	                  std::span<SrcType const> data)
	{
		void* ptr{};
		assert(buffer.capacity() >= sizeof(SrcType) * std::size(data));
		vmaMapMemory(buffer.allocator(), buffer.allocation(), &ptr);
		memcpy(ptr, std::data(std::as_const(data)), sizeof(SrcType) * std::size(data));
		vmaUnmapMemory(buffer.allocator(), buffer.allocation());
		return std::size(data);
	}

	template<auto BufferUsage, auto AllocationFlags, class SrcType, size_t N>
	requires(std::is_trivially_copyable_v<SrcType>&& is_host_visible(AllocationFlags)) size_t
	    sync_transfer(buffer<BufferUsage, AllocationFlags> const& buffer,
	                  std::span<SrcType const, N> data)
	{
		return sync_transfer(buffer, std::span<SrcType const>{data});
	}
}

#endif
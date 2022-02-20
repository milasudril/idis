//@	{
//@	 "dependencies_extra": [{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"},
//@		{"ref":"./image.o", "rel":"implementation"}
//@	]
//@	}

#ifndef IDIS_GPURES_IMAGE_HPP
#define IDIS_GPURES_IMAGE_HPP

#include "./format_to_format_id.hpp"

#include "engine/vk_init/allocator.hpp"

#include <type_traits>
#include <memory>
#include <cstring>
#include <cassert>

namespace idis::gpu_res
{
	class image_deleter
	{
	public:
		explicit image_deleter(VmaAllocator allocator, VmaAllocation allocation)
		    : m_allocator{allocator}
		    , m_allocation{allocation}
		{
		}

		void operator()(VkImage obj) const
		{
			if(obj != nullptr) { vmaDestroyImage(m_allocator, obj, m_allocation); }
		}

		VmaAllocation allocation() const { return m_allocation; }

		VmaAllocator allocator() const { return m_allocator; }

	private:
		VmaAllocator m_allocator;
		VmaAllocation m_allocation;
	};

	using image_handle = std::unique_ptr<std::remove_pointer_t<VkImage>, image_deleter>;

	struct image_descriptor
	{
		VkImageUsageFlagBits usage;
		VkFormat format;
		VkImageTiling tiling_mode;
	};

	image_handle create_image(VmaAllocator allocator,
	                          VmaAllocationCreateFlags alloc_flags,
	                          VkMemoryPropertyFlags alloc_requirements,
	                          image_descriptor const& descriptor,
	                          VkExtent2D dim);

	template<image_descriptor Descriptor, auto AllocationFlags, auto AllocationRequirements>
	class image
	{
	public:
		using handle_type                             = image_handle;
		static constexpr auto descriptor              = Descriptor;
		static constexpr auto allocation_flags        = AllocationFlags;
		static constexpr auto allocation_requirements = AllocationRequirements;
		using pixel_type                              = format_t<descriptor.format>;

		static constexpr bool check_template_arguments()
		{
			bool is_ok = true;
			if(descriptor.usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
			{
				is_ok = is_ok && descriptor.format == VK_FORMAT_D32_SFLOAT
				        && (AllocationRequirements & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			}

			if((AllocationFlags & VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT)
			   || (AllocationFlags & VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT))
			{
				is_ok = is_ok && !(AllocationRequirements & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
				is_ok = is_ok && (descriptor.tiling_mode == VK_IMAGE_TILING_LINEAR);
			}

			return is_ok;
		}

		static_assert(check_template_arguments());


		image(): m_handle{nullptr, image_deleter{nullptr, nullptr}} {}

		explicit image(std::reference_wrapper<vk_init::allocator const> allocator, VkExtent2D dim)
		    : image{allocator.get().handle(), dim}
		{
		}

		explicit image(VmaAllocator allocator, VkExtent2D dim)
		    : m_handle{
		        create_image(allocator, allocation_flags, allocation_requirements, descriptor, dim)}
		{
		}

		VkImage handle() const { return m_handle.get(); }

		VmaAllocator allocator() const { return m_handle.get_deleter().allocator(); }

		VmaAllocation allocation() const { return m_handle.get_deleter().allocation(); }

		VkDevice device() const
		{
			VmaAllocatorInfo info{};
			vmaGetAllocatorInfo(m_handle.get_deleter().allocator(), &info);
			return info.device;
		}

	private:
		handle_type m_handle;
	};

	template<auto TilingMode, auto AllocationFlags, auto AllocationRequirements>
	using basic_depth_buffer =
	    image<image_descriptor{
	              VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_FORMAT_D32_SFLOAT, TilingMode},
	          AllocationFlags,
	          AllocationRequirements>;

	using gpu_only_depth_buffer =
	    basic_depth_buffer<VK_IMAGE_TILING_OPTIMAL, 0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT>;

}

#endif
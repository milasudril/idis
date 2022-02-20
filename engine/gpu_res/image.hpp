//@	{
//@	 "dependencies_extra": [{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
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

	template<class PixelType,
	         auto ImageUsage,
	         auto TilingMode,
	         auto AllocationFlags,
	         auto AllocationRequirements>
	auto create_image(VmaAllocator allocator, VkExtent2D dim)
	{
		VkImageCreateInfo image_info{};
		image_info.sType       = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image_info.usage       = ImageUsage;
		image_info.imageType   = VK_IMAGE_TYPE_2D;
		image_info.format      = (ImageUsage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
                                    && std::is_same_v<PixelType, float>
		                             ? VK_FORMAT_D32_SFLOAT
		                             : format_id_v<PixelType>;
		image_info.extent      = VkExtent3D{dim.width, dim.height, 1};
		image_info.mipLevels   = 1;
		image_info.arrayLayers = 1;
		image_info.samples     = VK_SAMPLE_COUNT_1_BIT;
		image_info.tiling      = TilingMode;

		VmaAllocationCreateInfo alloc_info{};
		alloc_info.usage         = VMA_MEMORY_USAGE_AUTO;
		alloc_info.flags         = AllocationFlags;
		alloc_info.requiredFlags = AllocationRequirements;

		VkImage image{};
		VmaAllocation allocation{};
		if(auto res =
		       vmaCreateImage(allocator, &image_info, &alloc_info, &image, &allocation, nullptr);
		   res != VK_SUCCESS)
		{
			throw idis::exception{"create image", to_string(idis::vk_init::error{res})};
		}

		return image_handle{image, image_deleter{allocator, allocation}};
	}

	template<class PixelType,
	         auto ImageUsage,
	         auto TilingMode,
	         auto AllocationFlags,
	         auto AllocationRequirements>
	class image
	{
	public:
		static constexpr bool check_template_arguments()
		{
			bool is_ok = true;
			if(ImageUsage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
			{
				is_ok = is_ok && std::is_same_v<PixelType, float>;
			}

			if((AllocationFlags & VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT)
			   || (AllocationFlags & VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT))
			{
				is_ok = is_ok && !(AllocationRequirements & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
				is_ok = is_ok && (TilingMode == VK_IMAGE_TILING_LINEAR);
			}

			return is_ok;
		}

		static_assert(check_template_arguments());

		using handle_type                             = image_handle;
		static constexpr auto image_usage             = ImageUsage;
		static constexpr auto tiling_mode             = TilingMode;
		static constexpr auto allocation_flags        = AllocationFlags;
		static constexpr auto allocation_requirements = AllocationRequirements;

		image(): m_handle{nullptr, image_deleter{nullptr, nullptr}} {}

		explicit image(std::reference_wrapper<vk_init::allocator const> allocator, VkExtent2D dim)
		    : image{allocator.get().handle(), dim}
		{
		}

		explicit image(VmaAllocator allocator, VkExtent2D dim)
		    : m_handle{create_image<PixelType,
		                            ImageUsage,
		                            TilingMode,
		                            AllocationFlags,
		                            AllocationRequirements>(allocator, dim)}
		{
		}

		VkImage handle() const { return m_handle.get(); }

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

	template<auto TilingMode, auto AllocationFlags, auto AllocationRequirements>
	using basic_depth_buffer = image<float,
	                                 VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
	                                 TilingMode,
	                                 AllocationFlags,
	                                 AllocationRequirements>;

	using gpu_only_depth_buffer =
	    basic_depth_buffer<VK_IMAGE_TILING_OPTIMAL, 0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT>;

}

#endif
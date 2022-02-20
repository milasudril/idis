//@	{"target":{"name":"image.o"}}

#include "./image.hpp"

#include "engine/vk_init/error.hpp"

idis::gpu_res::image_handle idis::gpu_res::create_image(VmaAllocator allocator,
                                                        VmaAllocationCreateFlags alloc_flags,
                                                        VkMemoryPropertyFlags alloc_requirements,
                                                        image_descriptor const& descriptor,
                                                        VkExtent2D dim)
{
	VkImageCreateInfo image_info{};
	image_info.sType       = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_info.usage       = descriptor.usage;
	image_info.imageType   = VK_IMAGE_TYPE_2D;
	image_info.format      = descriptor.format;
	image_info.extent      = VkExtent3D{dim.width, dim.height, 1};
	image_info.mipLevels   = 1;
	image_info.arrayLayers = 1;
	image_info.samples     = VK_SAMPLE_COUNT_1_BIT;
	image_info.tiling      = descriptor.tiling_mode;

	VmaAllocationCreateInfo alloc_info{};
	alloc_info.usage         = VMA_MEMORY_USAGE_AUTO;
	alloc_info.flags         = alloc_flags;
	alloc_info.requiredFlags = alloc_requirements;

	VkImage image{};
	VmaAllocation allocation{};
	if(auto res = vmaCreateImage(allocator, &image_info, &alloc_info, &image, &allocation, nullptr);
	   res != VK_SUCCESS)
	{
		throw idis::exception{"create image", to_string(idis::vk_init::error{res})};
	}

	return image_handle{image, image_deleter{allocator, allocation}};
}
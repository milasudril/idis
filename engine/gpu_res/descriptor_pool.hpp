//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./descriptor_pool.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_DESCRIPTORPOOL_HPP
#define IDIS_GPURES_DESCRIPTORPOOL_HPP

#include "engine/vk_init/device.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class descriptor_pool_deleter
	{
	public:
		explicit descriptor_pool_deleter(VkDevice device): m_device{device} {}

		void operator()(VkDescriptorPool obj) const
		{
			if(obj != nullptr) { vkDestroyDescriptorPool(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	using descriptor_pool_handle =
	    std::unique_ptr<std::remove_pointer_t<VkDescriptorPool>, descriptor_pool_deleter>;

	descriptor_pool_handle create_descriptor_pool(VkDevice device,
	                                              uint32_t max_num_sets,
	                                              std::span<VkDescriptorPoolSize const> pool_sizes);

	class descriptor_pool
	{
	public:
		using handle_type = descriptor_pool_handle;

		descriptor_pool(): m_handle{nullptr, descriptor_pool_deleter{nullptr}} {}

		template<size_t NumDescriptors>
		explicit descriptor_pool(std::reference_wrapper<vk_init::device const> device,
		                         uint32_t max_num_sets,
		                         std::array<VkDescriptorPoolSize, NumDescriptors> const& pool_sizes)
		    : m_handle{create_descriptor_pool(device.get().handle(), max_num_sets, pool_sizes)}
		{
		}

		auto handle() const { return m_handle.get(); }

		void reset() { m_handle.reset(); }

		VkDevice device() const { return m_handle.get_deleter().device(); }

	private:
		handle_type m_handle;
	};
}

#endif
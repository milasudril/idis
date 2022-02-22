//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./descriptor_set_layout.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_DESCRIPTORSETLAYOUT_HPP
#define IDIS_GPURES_DESCRIPTORSETLAYOUT_HPP

#include "engine/vk_init/device.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class descriptor_set_layout_deleter
	{
	public:
		explicit descriptor_set_layout_deleter(VkDevice device): m_device{device} {}

		void operator()(VkDescriptorSetLayout obj) const
		{
			if(obj != nullptr) { vkDestroyDescriptorSetLayout(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	using descriptor_set_layout_handle =
	    std::unique_ptr<std::remove_pointer_t<VkDescriptorSetLayout>,
	                    descriptor_set_layout_deleter>;

	descriptor_set_layout_handle create_descriptor_set_layout(
	    VkDevice device,
	    VkDescriptorSetLayoutCreateFlags flags,
	    std::span<VkDescriptorSetLayoutBinding const> bindings);

	class descriptor_set_layout
	{
	public:
		using handle_type = descriptor_set_layout_handle;

		descriptor_set_layout(): m_handle{nullptr, descriptor_set_layout_deleter{nullptr}} {}

		template<size_t NumBindings>
		explicit descriptor_set_layout(
		    std::reference_wrapper<vk_init::device const> device,
		    VkDescriptorSetLayoutCreateFlags flags,
		    std::array<VkDescriptorSetLayoutBinding, NumBindings> const& bindings)
		    : m_handle{create_descriptor_set_layout(device.get().handle(), flags, bindings)}
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
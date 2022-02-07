//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./vk_shader_module.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_VKIMAGEVIEW_HPP
#define IDIS_GPURES_VKIMAGEVIEW_HPP

#include "engine/vk_init/device.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class vk_shader_module_deleter
	{
	public:
		explicit vk_shader_module_deleter(VkDevice device): m_device{device} {}

		void operator()(VkShaderModule obj) const
		{
			if(obj != nullptr) { vkDestroyShaderModule(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	using shader_module_handle =
	    std::unique_ptr<std::remove_pointer_t<VkShaderModule>, vk_shader_module_deleter>;

	shader_module_handle create_vk_shader_module(VkDevice device,
	                                             std::span<uint32_t const> spirv_data);

	template<auto PipelineStage>
	class vk_shader_module
	{
	public:
		static constexpr auto pipeline_stage = PipelineStage;

		using handle_type = shader_module_handle;

		vk_shader_module() = default;

		explicit vk_shader_module(vk_init::device& device, std::span<uint32_t const> spirv_data)
		    : vk_shader_module{device.handle(), spirv_data}
		{
		}

		explicit vk_shader_module(VkDevice device, std::span<uint32_t const> spirv_data)
		    : m_handle{create_vk_shader_module(device, spirv_data)}
		{
		}

		auto handle() const { return m_handle.get(); }

	private:
		handle_type m_handle;
	};
}

#endif
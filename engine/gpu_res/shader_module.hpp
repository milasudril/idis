//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./shader_module.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_VKSHADERMODULE_HPP
#define IDIS_GPURES_VKSHADERMODULE_HPP

#include "engine/vk_init/device.hpp"
#include "engine/shaders/shader_source.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	class shader_module_deleter
	{
	public:
		explicit shader_module_deleter(VkDevice device): m_device{device} {}

		void operator()(VkShaderModule obj) const
		{
			if(obj != nullptr) { vkDestroyShaderModule(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	using shader_module_handle =
	    std::unique_ptr<std::remove_pointer_t<VkShaderModule>, shader_module_deleter>;

	shader_module_handle create_shader_module(VkDevice device,
	                                          std::span<uint32_t const> spirv_data);

	template<auto PipelineStage>
	class shader_module
	{
	public:
		static constexpr auto pipeline_stage = PipelineStage;

		using handle_type = shader_module_handle;

		shader_module(): m_handle{nullptr, shader_module_deleter{nullptr}} {}

		template<class Container>
		explicit shader_module(vk_init::device& device,
		                       shaders::shader_source<Container, PipelineStage> spirv_data)
		    : shader_module{device.handle(), spirv_data}
		{
		}

		template<class Container>
		explicit shader_module(VkDevice device,
		                       shaders::shader_source<Container, PipelineStage> spirv_data)
		    : m_handle{create_shader_module(device, spirv_data.data)}
		{
		}

		auto handle() const { return m_handle.get(); }

	private:
		handle_type m_handle;
	};

	template<auto PipelineStage>
	auto get_shader_stage_info(shader_module<PipelineStage> const&&) = delete;

	template<auto PipelineStage>
	auto get_shader_stage_info(shader_module<PipelineStage> const& module)
	{
		VkPipelineShaderStageCreateInfo ret{};
		ret.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		ret.stage  = PipelineStage;
		ret.module = module.handle();
		ret.pName  = "main";
		return ret;
	}
}

#endif
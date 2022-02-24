//@	{
//@	 "dependencies_extra":
//@		[{"ref":"./pipeline.o", "rel":"implementation"},
//@		{"ref":"vulkan", "rel":"implementation", "origin":"pkg-config"}]
//@	}

#ifndef IDIS_GPURES_PIPELINE_HPP
#define IDIS_GPURES_PIPELINE_HPP

#include "./pipeline_descriptor.hpp"
#include "./render_pass.hpp"
#include "./shader_module.hpp"
#include "./pipeline_layout.hpp"
#include "./descriptor_set_layout.hpp"

#include <type_traits>
#include <memory>

namespace idis::gpu_res
{
	template<auto... Tags>
	using shader_module_set = std::tuple<shader_module<Tags>...>;

	struct shader_program_info
	{
		shader_module_set<VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT> modules;
		std::span<VkVertexInputBindingDescription const> input_bindings;
		std::span<VkVertexInputAttributeDescription const> input_attributes;
		pipeline_layout layout;
	};

	template<class ShaderDescriptor>
	shader_program_info make_shader_program_info(
	    std::reference_wrapper<vk_init::device const> device)
	{
		return shader_program_info{
		    {shader_module{device, ShaderDescriptor::vertex_shader()},
		     shader_module{device, ShaderDescriptor::fragment_shader()}},
		    std::span{input_binding_descriptor<ShaderDescriptor>::bindings},
		    std::span{input_binding_descriptor<ShaderDescriptor>::attributes},
		    pipeline_layout{device, std::span<descriptor_set_layout>{}}};
	}

	class pipeline_deleter
	{
	public:
		explicit pipeline_deleter(VkDevice device): m_device{device} {}

		void operator()(VkPipeline obj) const
		{
			if(obj != nullptr) { vkDestroyPipeline(m_device, obj, nullptr); }
		}

		VkDevice device() const { return m_device; }

	private:
		VkDevice m_device;
	};

	using pipeline_handle_type =
	    std::unique_ptr<std::remove_pointer_t<VkPipeline>, pipeline_deleter>;

	pipeline_handle_type create_pipeline(VkDevice device,
	                                     pipeline_descriptor const& descriptor,
	                                     shader_program_info const& shader_program,
	                                     VkRenderPass matching_rp);

	template<class ShaderDescriptor>
	class pipeline
	{
	public:
		using handle_type       = pipeline_handle_type;
		using shader_descriptor = ShaderDescriptor;

		pipeline(): m_handle{nullptr, pipeline_deleter{nullptr}} {}


		explicit pipeline(std::reference_wrapper<vk_init::device const> device,
		                  pipeline_descriptor const& descriptor,
		                  render_pass const& matching_rp)
		    : m_shader_program{make_shader_program_info<ShaderDescriptor>(device)}
		    , m_handle{create_pipeline(
		          device.get().handle(), descriptor, m_shader_program, matching_rp.handle())}
		{
		}

		auto handle() const { return m_handle.get(); }

		void reset() { m_handle.reset(); }

	private:
		shader_program_info m_shader_program;
		handle_type m_handle;
	};
}

#endif
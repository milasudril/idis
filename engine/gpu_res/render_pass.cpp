//@	{"target":{"name":"render_pass.o"}}

#include "./render_pass.hpp"

#include "engine/error_handling/exception.hpp"
#include "engine/vk_init/error.hpp"

namespace
{
	auto create_render_pass(VkDevice dev, VkFormat output_format)
	{
		VkAttachmentDescription color_attachment{};
		color_attachment.format         = output_format;
		color_attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
		color_attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
		color_attachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
		color_attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		color_attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
		color_attachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference color_attachment_ref{};
		color_attachment_ref.attachment = 0;
		color_attachment_ref.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments    = &color_attachment_ref;

		VkSubpassDependency dependency{};
		dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass    = 0;
		dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo render_pass_info{};
		render_pass_info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		render_pass_info.attachmentCount = 1;
		render_pass_info.pAttachments    = &color_attachment;
		render_pass_info.subpassCount    = 1;
		render_pass_info.pSubpasses      = &subpass;
		render_pass_info.dependencyCount = 1;
		render_pass_info.pDependencies   = &dependency;

		VkRenderPass ret{};
		if(auto res = vkCreateRenderPass(dev, &render_pass_info, nullptr, &ret); res != VK_SUCCESS)
		{
			throw idis::exception{"create render pass", to_string(idis::vk_init::error{res})};
		}

		return idis::gpu_res::render_pass::handle_type{ret,
		                                               idis::gpu_res::render_pass_deleter{dev}};
	}
}

idis::gpu_res::render_pass::render_pass(VkDevice dev, VkFormat output_format)
    : m_handle{create_render_pass(dev, output_format)}
{
}
//@	{"target":{"name":"format_to_format_id.test"}}

#include "./format_to_format_id.hpp"

#include "testfwk/testfwk.hpp"

#include <type_traits>

TESTCASE(idis_gpures_format_to_format_id_all_different)
{
	EXPECT_NE(idis::gpu_res::format_id_v<idis::vec2i_t>, idis::gpu_res::format_id_v<idis::vec2f_t>);
	EXPECT_NE(idis::gpu_res::format_id_v<idis::vec2i_t>, idis::gpu_res::format_id_v<idis::vec4i_t>);
	EXPECT_NE(idis::gpu_res::format_id_v<idis::vec2i_t>, idis::gpu_res::format_id_v<idis::vec4f_t>);
	EXPECT_NE(idis::gpu_res::format_id_v<idis::vec2f_t>, idis::gpu_res::format_id_v<idis::vec4i_t>);
	EXPECT_NE(idis::gpu_res::format_id_v<idis::vec2f_t>, idis::gpu_res::format_id_v<idis::vec4f_t>);
	EXPECT_NE(idis::gpu_res::format_id_v<idis::vec4i_t>, idis::gpu_res::format_id_v<idis::vec4f_t>);
}

TESTCASE(idis_gpures_format_to_format_id_all_expected_vals)
{
	EXPECT_EQ(idis::gpu_res::format_id_v<idis::vec2i_t>, VK_FORMAT_R32G32_SINT);
	EXPECT_EQ(idis::gpu_res::format_id_v<idis::vec4i_t>, VK_FORMAT_R32G32B32A32_SINT);
	EXPECT_EQ(idis::gpu_res::format_id_v<idis::vec2f_t>, VK_FORMAT_R32G32_SFLOAT);
	EXPECT_EQ(idis::gpu_res::format_id_v<idis::vec4f_t>, VK_FORMAT_R32G32B32A32_SFLOAT);
}

TESTCASE(idis_gpures_format_to_format_id_inverse_1)
{
	static_assert(std::is_same_v<idis::gpu_res::format_t<idis::gpu_res::format_id_v<idis::vec2i_t>>,
	                             idis::vec2i_t>);
	static_assert(std::is_same_v<idis::gpu_res::format_t<idis::gpu_res::format_id_v<idis::vec4i_t>>,
	                             idis::vec4i_t>);
	static_assert(std::is_same_v<idis::gpu_res::format_t<idis::gpu_res::format_id_v<idis::vec2f_t>>,
	                             idis::vec2f_t>);
	static_assert(std::is_same_v<idis::gpu_res::format_t<idis::gpu_res::format_id_v<idis::vec4f_t>>,
	                             idis::vec4f_t>);
}

TESTCASE(idis_gpures_format_to_format_id_inverse_2)
{
	EXPECT_EQ(idis::gpu_res::format_id_v<idis::gpu_res::format_t<VK_FORMAT_R32G32_SINT>>,
	          VK_FORMAT_R32G32_SINT);
	EXPECT_EQ(idis::gpu_res::format_id_v<idis::gpu_res::format_t<VK_FORMAT_R32G32B32A32_SINT>>,
	          VK_FORMAT_R32G32B32A32_SINT);
	EXPECT_EQ(idis::gpu_res::format_id_v<idis::gpu_res::format_t<VK_FORMAT_R32G32_SFLOAT>>,
	          VK_FORMAT_R32G32_SFLOAT);
	EXPECT_EQ(idis::gpu_res::format_id_v<idis::gpu_res::format_t<VK_FORMAT_R32G32B32A32_SFLOAT>>,
	          VK_FORMAT_R32G32B32A32_SFLOAT);
}

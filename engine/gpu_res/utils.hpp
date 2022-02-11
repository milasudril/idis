#ifndef IDIS_GPURES_UTILS_HPP
#define IDIS_GPURES_UTILS_HPP

#include "./swapchain.hpp"
#include "./image_view.hpp"
#include "./framebuffer.hpp"

#include <vector>
#include <algorithm>

namespace idis::gpu_res
{
	inline std::vector<image_view> create_image_views_from(swapchain const& src)
	{
		std::vector<image_view> ret;
		std::ranges::transform(src.get_images(),
		                       std::back_inserter(ret),
		                       [dev = src.device(), format = src.image_format()](auto item) {
			                       return image_view{dev, item, format};
		                       });
		return ret;
	}

	inline std::vector<framebuffer> create_framebuffers_from(idis::init::device& device,
															 render_pass& rp,
															 std::span<image_view> img_views)
	{
		std::vector<framebuffer> ret;
		std::ranges::transform(img_views, std::back_inserter(ret), [dev = device.handle(),
			rp_handle = rp.handle()
		](auto& item){
			return framebuffer{dev, rp_handle, item};
		});

		return ret;
	}
}

#endif
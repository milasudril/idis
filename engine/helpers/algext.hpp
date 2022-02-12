#ifndef IDIS_HELPERS_ALGEXT_HPP
#define IDIS_HELPERS_ALGEXT_HPP

#include <vector>
#include <ranges>

namespace idis
{
	template<class ResultItem, class Container>
	std::vector<ResultItem> split(Container const& src, typename Container::value_type delimiter)
	{
		std::vector<ResultItem> ret;
		auto ptr  = std::begin(src);
		auto prev = ptr;
		while(ptr != std::end(src))
		{
			if(*ptr == delimiter)
			{
				ret.push_back(ResultItem{prev, ptr});
				++ptr;
				prev = ptr;
			}
			else
			{
				++ptr;
			}
		}

		ret.push_back(ResultItem{prev, ptr});

		return ret;
	}

	template<class Container>
	auto split(Container const& src, typename Container::value_type delimiter)
	{
		return split<Container, Container>(src, delimiter);
	}

	template<size_t Start, class... T>
	void inc_all(std::tuple<T...>& vals)
	{
		if constexpr(Start == std::tuple_size_v<std::tuple<T...>>) { return; }
		else
		{
			++std::get<Start>(vals);
			return inc_all<Start + 1>(vals);
		}
	}

	template<class Callback, std::ranges::range First, std::ranges::range... Ranges>
	void for_each(Callback&& cb, First&& range, Ranges&&... ranges)
	{
		auto ptrs = std::tuple{std::begin(range), std::begin(ranges)...};
		auto end  = std::end(range);
		while(std::get<0>(ptrs) != end)
		{
			std::apply(cb,
			           std::apply([](auto&&... ptrs)
			                      { return std::tuple<decltype(*ptrs)...>{*ptrs...}; },
			                      ptrs));
			inc_all<0>(ptrs);
		}
	}
}

#endif
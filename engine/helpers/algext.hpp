#ifndef IDIS_HELPERS_ALGEXT
#define IDIS_HELPERS_ALGEXT

#include <vector>

namespace idis
{
	template<class ResultItem, class Container>
	std::vector<ResultItem> split(Container const& src, typename Container::value_type delimiter)
	{
		std::vector<ResultItem> ret;
		auto ptr = std::begin(src);
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
			{ ++ptr; }
		}

		ret.push_back(ResultItem{prev, ptr});

		return ret;
	}

	template<class Container>
	auto split(Container const& src, typename Container::value_type delimiter)
	{ return split<Container, Container>(src, delimiter); }
}

#endif
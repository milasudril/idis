#ifndef IDIS_UTILS_PAIR_HPP
#define IDIS_UTILS_PAIR_HPP

namespace idis
{
	template<class T, class U = T>
	struct pair
	{
		T first;
		U second;
	};
}

#endif
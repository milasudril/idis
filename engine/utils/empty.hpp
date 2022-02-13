#ifndef IDIS_UTILS_EMPTY_HPP
#define IDIS_UTILS_EMPTY_HPP

#include <type_traits>

namespace idis
{
	template<class T>
	using empty = std::type_identity<T>;
}

#endif

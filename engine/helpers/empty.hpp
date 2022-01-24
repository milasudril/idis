#ifndef IDIS_HELPERS_EMPTY_HPP
#define IDIS_HELPERS_EMPTY_HPP

#include <type_traits>

namespace idis
{
	template<class T>
	using empty = std::type_identity<T>;
}

#endif

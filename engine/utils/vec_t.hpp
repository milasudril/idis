#ifndef IDIS_UTILS_VECT_HPP
#define IDIS_UTILS_VECT_HPP

#include <cstddef>
#include <cmath>

namespace idis
{
	template<class T, size_t N>
	using vec_t [[gnu::vector_size(N * sizeof(T))]] = T;

	template<class T>
	constexpr size_t vector_size_v = 0;

	template<class T>
	struct scalar;

	template<class T>
	using scalar_t = scalar<T>::type;

	template<class T>
	constexpr bool is_native_vector()
	{
		return vector_size_v<T> != 0;
	}

	template<class T>
	requires(is_native_vector<T>()) constexpr auto dot(T a, T b)
	{
		scalar_t<T> ret{};
		for(size_t k = 0; k != vector_size_v<T>; ++k)
		{
			ret += a[k] * b[k];
		}
		return ret;
	}

	template<class T>
	requires(is_native_vector<T>()) constexpr auto length_squared(T a) { return dot(a, a); }

	template<class T>
	requires(is_native_vector<T>()
	         && std::is_floating_point_v<scalar_t<T>>) constexpr auto length(T a)
	{
		return std::sqrt(length_squared(a));
	}

	template<class T>
	requires(is_native_vector<T>()) constexpr auto normalized(T a) { return a / length(a); }

	template<class T>
	requires(is_native_vector<T>()
	         && std::is_floating_point_v<scalar_t<T>>) constexpr auto lerp(T a, T b, scalar_t<T> t)
	{
		return (t - static_cast<scalar_t<T>>(1)) * a + t * b;
	}

	template<class To, class From>
	requires(vector_size_v<To> == 4 && vector_size_v<From> == 4) constexpr auto vector_cast(From a)
	{
		return To{static_cast<scalar_t<To>>(a[0]),
		          static_cast<scalar_t<To>>(a[1]),
		          static_cast<scalar_t<To>>(a[2]),
		          static_cast<scalar_t<To>>(a[3])};
	}

	template<class To, class From>
	requires(vector_size_v<To> == 2 && vector_size_v<From> == 2) constexpr auto vector_cast(From a)
	{
		return To{static_cast<scalar_t<To>>(a[0]), static_cast<scalar_t<To>>(a[1])};
	}

	template<class T>
	using vec2_t = vec_t<T, 2>;

	template<class T>
	using vec4_t = vec_t<T, 4>;

	using vec2i_t = vec2_t<int>;
	using vec2f_t = vec2_t<float>;
	using vec4i_t = vec4_t<int>;
	using vec4f_t = vec4_t<float>;

	template<>
	constexpr auto vector_size_v<vec2i_t> = 2;
	template<>
	constexpr auto vector_size_v<vec2f_t> = 2;
	template<>
	constexpr auto vector_size_v<vec4i_t> = 4;
	template<>
	constexpr auto vector_size_v<vec4f_t> = 4;

	template<>
	struct scalar<vec2i_t>
	{
		using type = int;
	};

	template<>
	struct scalar<vec4i_t>
	{
		using type = int;
	};
	template<>
	struct scalar<vec2f_t>
	{
		using type = float;
	};

	template<>
	struct scalar<vec4f_t>
	{
		using type = float;
	};
}


#endif
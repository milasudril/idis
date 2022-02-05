#ifndef IDIS_HELPERS_HEXFORMAT_HPP
#define IDIS_HELPERS_HEXFORMAT_HPP

#include <concepts>
#include <string>
#include <algorithm>

namespace idis
{
	template<class T>
	requires(std::is_integral_v<T> || std::is_pointer_v<T>) class hexformat
	{
	public:
		constexpr explicit hexformat(T value): m_value{value} {}

		constexpr auto value() const { return m_value; }

		constexpr auto operator<=>(hexformat const& other) const = default;

	private:
		T m_value;
	};

	template<std::integral T>
	std::string to_string(hexformat<T> val)
	{
		auto unsigned_val = static_cast<std::make_unsigned_t<T>>(val.value());

		std::string ret;
		decltype(unsigned_val) mask = 0xf;
		decltype(mask) k            = 0;
		while(mask != 0)
		{
			auto const nibble = static_cast<char>((unsigned_val & mask) >> (4 * k));
			ret += (nibble < 10) ? (nibble + '0') : (nibble - 10 + 'a');
			mask <<= 4;
			++k;
		}
		std::ranges::reverse(std::begin(ret), std::end(ret));
		return ret;
	}

	template<class T>
	requires(std::is_pointer_v<T>) std::string to_string(hexformat<T> val)
	{
		return to_string(hexformat{reinterpret_cast<uintptr_t>(val.value())});
	}
}

#endif
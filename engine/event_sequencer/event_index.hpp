#ifndef IDIS_EVENTSEQUENCER_EVENTINDEX_HPP
#define IDIS_EVENTSEQUENCER_EVENTINDEX_HPP

#include <string>

namespace idis::seq
{
	class event_index
	{
	public:
		constexpr event_index(): m_value{0} {}

		constexpr explicit event_index(int64_t val): m_value{val} {}

		constexpr auto operator<=>(event_index const& other) const = default;

		constexpr auto value() const { return m_value; }

		constexpr void next() { ++m_value; }

	private:
		int64_t m_value;
	};

	inline std::string to_string(event_index t) { return std::to_string(t.value()); }
}

#endif
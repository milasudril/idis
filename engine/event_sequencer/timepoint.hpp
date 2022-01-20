#ifndef IDIS_EVENTSEQUENCER_TIMEPOINT_HPP
#define IDIS_EVENTSEQUENCER_TIMEPOINT_HPP

#include <cstdint>
#include <string>

namespace idis::seq
{
	class timepoint
	{
	public:
		constexpr explicit timepoint(int64_t val): m_value{val} {}

		constexpr auto operator<=>(timepoint const& other) const = default;

		constexpr auto value() const { return m_value; }

		constexpr void step() { ++m_value; }

	private:
		int64_t m_value;
	};

	constexpr int64_t timepoints_per_second = 60;

	constexpr int64_t operator-(timepoint a, timepoint b) { return a.value() - b.value(); }

	constexpr auto operator+(timepoint a, int64_t t)
	{
		return timepoint{a.value() + t};
	}

	constexpr auto operator-(timepoint a, int64_t t)
	{
		return timepoint{a.value() - t};
	}

	inline std::string to_string(timepoint t) { return std::to_string(t.value()); }

	inline double seconds_between(timepoint a, timepoint b)
	{
		return static_cast<double>(a - b) / static_cast<double>(timepoints_per_second);
	}

	inline double seconds_since_start(timepoint a)
	{
		return seconds_between(timepoint{0}, a);
	}
}

#endif
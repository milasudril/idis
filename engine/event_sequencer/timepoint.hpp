#ifndef IDIS_EVENTSEQUENCER_TIMEPOINT_HPP
#define IDIS_EVENTSEQUENCER_TIMEPOINT_HPP

#include <cstdint>
#include <string>

namespace idis::seq
{
	class timepoint
	{
	public:
		constexpr explicit timepoint(uint64_t val): m_value{val} {}

		constexpr auto operator<=>(timepoint const& other) const = default;

		constexpr auto value() const { return m_value; }

		constexpr void tick() { ++m_value; }

	private:
		uint64_t m_value;
	};

	constexpr uint64_t timepoints_per_second = 60;

	constexpr int64_t operator-(timepoint a, timepoint b) { return a.value() - b.value(); }

	constexpr auto operator+(timepoint a, int64_t t)
	{
		return timepoint{static_cast<uint64_t>(a.value() + t)};
	}

	constexpr auto operator-(timepoint a, int64_t t)
	{
		return timepoint{static_cast<uint64_t>(a.value() - t)};
	}

	inline std::string to_string(timepoint t) { return std::to_string(t); }

	double seconds_between(timepoint a, timepoint b)
	{
		return static_cast<double>(a - b) / static_cast<double>(timepoints_per_second);
	}

	double seconds_since_start(timepoint a)
	{
		return seconds_between(timepoint{}, a);
	}
}

#endif
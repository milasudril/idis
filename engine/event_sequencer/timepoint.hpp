#ifndef IDIS_EVENTSEQUENCER_TIMEPOINT_HPP
#define IDIS_EVENTSEQUENCER_TIMEPOINT_HPP

#include "helpers/integer_mixin.hpp"
#include "helpers/empty.hpp"

#include <cstdint>
#include <string>
#include <chrono>

namespace idis::seq
{
	class tick: public integer_mixin<int64_t, tick>
	{
	public:
		using base = integer_mixin<int64_t, tick>;
		using base::base;

		constexpr auto count() const { return value(); }
	};

	constexpr auto ticks_per_sec = static_cast<tick::rep>(60);

	inline std::string to_string(tick val)
	{
		auto const secs  = val / ticks_per_sec;
		auto const ticks = abs(val - ticks_per_sec * secs);
		if(ticks < tick{10})
		{
			return std::to_string(secs.value()).append(":0").append(std::to_string(ticks.value()));
		}
		else
		{
			return std::to_string(secs.value()).append(":").append(std::to_string(ticks.value()));
		}
	}

	constexpr auto to_seconds(tick val)
	{
		return std::chrono::duration<double>{static_cast<double>(val.value()) / ticks_per_sec};
	}

	constexpr auto from(empty<tick>, std::chrono::duration<double> d)
	{
		return tick{static_cast<tick::rep>(d.count() * static_cast<double>(ticks_per_sec))};
	}

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

	constexpr auto operator+(timepoint a, int64_t t) { return timepoint{a.value() + t}; }

	constexpr auto operator-(timepoint a, int64_t t) { return timepoint{a.value() - t}; }

	inline std::string to_string(timepoint t) { return std::to_string(t.value()); }

	inline double seconds_between(timepoint a, timepoint b)
	{
		return static_cast<double>(a - b) / static_cast<double>(timepoints_per_second);
	}

	inline double seconds_since_start(timepoint a) { return seconds_between(timepoint{0}, a); }
}

#endif

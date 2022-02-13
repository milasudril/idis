#ifndef IDIS_EVENTSEQUENCER_TIMEPOINT_HPP
#define IDIS_EVENTSEQUENCER_TIMEPOINT_HPP

#include "engine/utils/integer_mixin.hpp"
#include "engine/utils/empty.hpp"

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

	constexpr auto seconds_per_tick = std::chrono::duration<int64_t, std::ratio<1, 60>>{1};

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
		constexpr timepoint(): m_time_since_epoch{tick::zero()} {}

		constexpr explicit timepoint(tick val): m_time_since_epoch{val} {}

		constexpr auto operator<=>(timepoint const& other) const = default;

		constexpr auto next()
		{
			++m_time_since_epoch;
			return *this;
		}

		constexpr auto time_since_epoch() const { return m_time_since_epoch; }

		constexpr auto& operator+=(tick amount)
		{
			m_time_since_epoch += amount;
			return *this;
		}

		constexpr auto& operator-=(tick amount)
		{
			m_time_since_epoch -= amount;
			return *this;
		}

	private:
		tick m_time_since_epoch;
	};

	constexpr auto operator-(timepoint a, timepoint b)
	{
		return a.time_since_epoch() - b.time_since_epoch();
	}

	constexpr auto operator+(timepoint a, tick val) { return a += val; }

	constexpr auto operator-(timepoint a, tick val) { return a -= val; }

	inline std::string to_string(timepoint t) { return to_string(t.time_since_epoch()); }

	inline auto seconds_between(timepoint a, timepoint b) { return to_seconds(b - a); }

	inline auto seconds_since_start(timepoint a) { return seconds_between(timepoint{}, a); }
}

#endif

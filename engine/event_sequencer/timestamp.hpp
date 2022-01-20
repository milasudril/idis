#ifndef IDIS_EVENTSEQUENCER_TIMESTAMP_HPP
#define IDIS_EVENTSEQUENCER_TIMESTAMP_HPP

#include "./timepoint.hpp"
#include "./event_index.hpp"

namespace idis::seq
{
	class timestamp
	{
	public:
		static constexpr uint64_t events_per_frame = 0xffffff;
		static constexpr uint64_t event_index_bits = 24;

		constexpr explicit timestamp(timepoint frame, event_index i)
		    : m_value{(frame.value() << event_index_bits) | i.value()}
		{
		}

		constexpr timepoint frame() const { return timepoint{m_value >> event_index_bits}; }

		constexpr event_index index() const
		{
			return event_index{static_cast<int>(m_value & events_per_frame)};
		}

		constexpr auto operator<=>(timestamp const&) const = default;

	private:
		int64_t m_value;
	};

	inline std::string to_string(timestamp t)
	{
		return to_string(t.frame()).append(":").append(to_string(t.index()));
	}
}
#endif
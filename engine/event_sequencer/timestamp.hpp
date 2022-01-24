#ifndef IDIS_EVENTSEQUENCER_TIMESTAMP_HPP
#define IDIS_EVENTSEQUENCER_TIMESTAMP_HPP

#include "./timepoint.hpp"
#include "./event_index.hpp"

namespace idis::seq
{
	class timestamp
	{
	public:
		constexpr explicit timestamp(timepoint frame, event_index i): m_frame{frame}, m_index{i} {}

		constexpr auto frame() const { return m_frame; }

		constexpr auto index() const { return m_index; }

		constexpr auto operator<=>(timestamp const&) const = default;

	private:
		timepoint m_frame;
		event_index m_index;
	};

	inline std::string to_string(timestamp t)
	{
		return to_string(t.frame()).append(" (").append(to_string(t.index())).append(")");
	}
}
#endif

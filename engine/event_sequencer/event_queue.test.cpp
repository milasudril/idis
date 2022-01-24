//@	{"target":{"name":"event_queue.test"}}

#include "./event_queue.hpp"

#include "ext_proj/testfwk/testfwk.hpp"

#include <array>
#include <algorithm>

namespace
{
	class action
	{
	public:
		explicit action(size_t id, std::vector<size_t>& out): m_id{id}, m_out{out} {}

		void operator()() { m_out.get().push_back(m_id); }

	private:
		size_t m_id;
		std::reference_wrapper<std::vector<size_t>> m_out;
	};
}

TESTCASE(idis_seq_event_queue_push_and_drain_seq)
{
	idis::seq::event_queue events;
	EXPECT_EQ(events.empty(), true);

	std::array<idis::seq::timepoint, 16> timepoints{idis::seq::timepoint{idis::seq::tick{2}},
	                                                idis::seq::timepoint{idis::seq::tick{3}},
	                                                idis::seq::timepoint{idis::seq::tick{0}},
	                                                idis::seq::timepoint{idis::seq::tick{1}},
	                                                idis::seq::timepoint{idis::seq::tick{7}},
	                                                idis::seq::timepoint{idis::seq::tick{4}},
	                                                idis::seq::timepoint{idis::seq::tick{5}},
	                                                idis::seq::timepoint{idis::seq::tick{6}},
	                                                idis::seq::timepoint{idis::seq::tick{7}},
	                                                idis::seq::timepoint{idis::seq::tick{3}},
	                                                idis::seq::timepoint{idis::seq::tick{2}},
	                                                idis::seq::timepoint{idis::seq::tick{1}},
	                                                idis::seq::timepoint{idis::seq::tick{4}},
	                                                idis::seq::timepoint{idis::seq::tick{6}},
	                                                idis::seq::timepoint{idis::seq::tick{5}},
	                                                idis::seq::timepoint{idis::seq::tick{0}}};

	std::ranges::sort(timepoints);

	std::vector<size_t> out;
	std::ranges::for_each(timepoints,
	                      [&events, k = static_cast<size_t>(0), &out](auto t) mutable
	                      {
		                      events.push(t, action{k, out});
		                      ++k;
	                      });

	EXPECT_EQ(events.empty(), false);

	drain(events, idis::seq::timepoint{idis::seq::tick{7}});

	EXPECT_EQ(events.empty(), true);
	EXPECT_EQ(std::size(out), std::size(timepoints));
	for(size_t k = 0; k != std::size(out); ++k)
	{
		EXPECT_EQ(k, out[k]);
	}
}
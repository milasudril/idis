//@ {"target":{"name":"event.test"}}

#include "./event.hpp"

#include "ext_proj/testfwk/testfwk.hpp"

#include <functional>

TESTCASE(idis_seq_event_create_and_verify)
{
	int val = 0;
	auto const t_exp =
	    idis::seq::timestamp{idis::seq::timepoint{idis::seq::tick{1}}, idis::seq::event_index{2}};
	idis::seq::event e1{t_exp, [x = std::ref(val)]() { ++x.get(); }};

	EXPECT_EQ(val, 0);
	EXPECT_EQ(e1.expire_time(), t_exp);
	EXPECT_EQ(e1.expire_time().frame(), idis::seq::timepoint{idis::seq::tick{1}});
	EXPECT_EQ(e1.expire_time().index(), idis::seq::event_index{2});

	EXPECT_EQ(e1.has_expired(idis::seq::timepoint{idis::seq::tick{0}}), false);
	EXPECT_EQ(e1.has_expired(idis::seq::timepoint{idis::seq::tick{1}}), true);
	EXPECT_EQ(e1.has_expired(idis::seq::timepoint{idis::seq::tick{2}}), true);

	e1.fire();
	EXPECT_EQ(val, 1);
}
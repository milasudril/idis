//@{"target":{"name":"action.test"}}

#include "./action.hpp"

#include "ext_proj/testfwk/testfwk.hpp"

TESTCASE(idis_seq_action_create_and_call)
{
	bool called = false;
	idis::seq::action action{[&called]() { called = true; }};
	action();
	EXPECT_EQ(called, true);
}
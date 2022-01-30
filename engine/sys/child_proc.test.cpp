//@	{"target":{"name":"child_proc.test"}}

#include "./child_proc.hpp"

#include "testfwk/testfwk.hpp"

#include <cassert>

TESTCASE(idis_sys_childproc_launch_and_exit)
{
	idis::sys::child_proc proc{"My child",
	                           []()
	                           {
		                           fprintf(stderr, "Hello, World");
		                           return 1;
	                           }};

	auto res = proc.get_result();
	EXPECT_EQ(res.status(), idis::sys::proc_wait_status{idis::sys::proc_exit_status{1}});
}

TESTCASE(idis_sys_childproc_launch_and_throw_unknown_exception)
{
	idis::sys::child_proc proc{"My child", []() -> int { throw "This is bad"; }};

	auto res = proc.get_result();
	EXPECT_EQ(res.status(), idis::sys::proc_wait_status{idis::sys::proc_exit_status{255}});
}

TESTCASE(idis_sys_childproc_launch_and_throw_known_exception)
{
	idis::sys::child_proc proc{"My child",
	                           []() -> int { throw std::runtime_error{"This is bad"}; }};

	auto res = proc.get_result();
	EXPECT_EQ(res.status(), idis::sys::proc_wait_status{idis::sys::proc_exit_status{255}});
}

TESTCASE(idis_sys_childproc_launch_and_abort)
{
	idis::sys::child_proc proc{"My child",
	                           []() -> int
	                           {
#undef NDEBUG
		                           assert(false);
	                           }};

	auto res = proc.get_result();
	EXPECT_EQ(res.status(), idis::sys::proc_wait_status{idis::sys::proc_term_signal{6}});
}
//@	{"target":{"name":"periodic_timer.test"}}

#include "./periodic_timer.hpp"

#include "testfwk/testfwk.hpp"

#include <thread>

TESTCASE(idis_timer_duration_to_timespec)
{
	auto res = idis::timer::to_timespec(std::chrono::duration<double>{1.75});

	EXPECT_EQ(res.tv_sec, 1);
	EXPECT_EQ(res.tv_nsec, 750'000'000);
}

TESTCASE(idis_timer_periodic_timer_expire_twice_before_read)
{
	idis::timer::periodic_timer timer{std::chrono::seconds{1}};
	timer.wait();  // wait for initial expire event
	std::this_thread::sleep_for(std::chrono::duration<double>{2.25});
	auto t0 = std::chrono::steady_clock::now();
	auto n  = timer.wait();
	auto t1 = std::chrono::steady_clock::now();
	EXPECT_EQ(n, 2);
	EXPECT_LT(std::chrono::duration<double>(t1 - t0).count(), 1.0);
}

TESTCASE(idis_timer_periodic_timer_check_period)
{
	idis::timer::periodic_timer timer{std::chrono::seconds{1}};
	timer.wait();
	auto t0 = std::chrono::steady_clock::now();
	auto n  = timer.wait();
	auto t1 = std::chrono::steady_clock::now();
	EXPECT_EQ(n, 1);
	EXPECT_LT(std::abs(std::chrono::duration<double>(t1 - t0).count() - 1.0), 0.25);
}
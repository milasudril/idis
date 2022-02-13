#ifndef IDIS_SYS_PERIODICTIMER_HPP
#define IDIS_SYS_PERIODICTIMER_HPP

#include "./fd.hpp"
#include "./error_code.hpp"
#include "engine/error_handling/exception.hpp"

#include <chrono>
#include <sys/timerfd.h>
#include <unistd.h>

namespace idis::sys
{
	template<class Rep, class Period>
	timespec to_timespec(std::chrono::duration<Rep, Period> period)
	{
		auto const t_sec = std::chrono::duration_cast<std::chrono::seconds>(period);
		auto const n_sec = period - std::chrono::duration_cast<std::chrono::nanoseconds>(t_sec);

		timespec ret{};
		ret.tv_sec  = t_sec.count();
		ret.tv_nsec = n_sec.count();
		return ret;
	}

	class periodic_timer
	{
	public:
		template<class Rep, class Period>
		explicit periodic_timer(std::chrono::duration<Rep, Period> period)
		    : m_fd{timerfd_create(CLOCK_MONOTONIC, 0)}
		{
			if(m_fd == nullptr) { throw exception{"create periodic timer", to_string(error_code{errno})}; }

			itimerspec spec{};
			spec.it_interval = to_timespec(period);
			spec.it_value    = spec.it_interval;
			timerfd_settime(m_fd.get(), 0, &spec, nullptr);
		}

		size_t wait()
		{
			size_t expire_count{};
			unused(read(m_fd.get(), &expire_count, sizeof(expire_count)));
			return expire_count;
		}

	private:
		fd_handle m_fd;
		static_assert(sizeof(m_fd) == sizeof(int));
	};
}

#endif
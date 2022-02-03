//@	{"target":{"name":"child_proc.o"}}

#include "./child_proc.hpp"

#define SIGCASE(signal)                                                                            \
	case signal: return #signal

std::string idis::sys::to_string(proc_term_signal val)
{
	switch(val.value())
	{
		SIGCASE(SIGABRT);
		SIGCASE(SIGBUS);
		SIGCASE(SIGILL);
		SIGCASE(SIGSEGV);
		SIGCASE(SIGXCPU);
		SIGCASE(SIGXFSZ);
		SIGCASE(SIGFPE);
		SIGCASE(SIGSYS);

		default: return "<UNKNOWN>";
	}
}

void idis::sys::child_proc::fetch_result() const
{
	std::vector<std::byte> errmsg;
	std::array<std::byte, 4096> buffer{};
	while(true)
	{
		auto n_read = read(m_stderr.read_fd(), std::data(buffer), std::size(buffer));
		if(n_read == -1 && errno == EAGAIN) { continue; }
		if(n_read == 0) { break; }
		errmsg.insert(std::end(errmsg), std::begin(buffer), std::begin(buffer) + n_read);
	}

	int wres;
	waitpid(m_pid, &wres, 0);
	m_res = process_result{proc_wait_status{wres}, std::move(errmsg)};
	m_pid = -1;
}

std::string idis::sys::get_error_message(process_result const& res)
{
	std::string ret{};
	auto const& data = res.error_log();
	auto ptr         = std::begin(data);
	auto const end   = std::end(data);
	while(ptr != end && *ptr != static_cast<std::byte>(0))
	{
		ret += static_cast<char>(*ptr);
		++ptr;
	}
	return ret;
}

std::string idis::sys::to_string(process_result const& res)
{
	if(has_signal(res.status()))
	{
		return std::string{"Process was terminated by a "}
		    .append(to_string(res.status()))
		    .append("\n\n")
		    .append(get_error_message(res));
	}
	else
	{
		return std::string{"Process returned "}
		    .append(to_string(res.status()))
		    .append("\n\n")
		    .append(get_error_message(res));
	}
}
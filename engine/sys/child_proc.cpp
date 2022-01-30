//@	{"target":{"name":"child_proc.o"}}

#include "./child_proc.hpp"

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
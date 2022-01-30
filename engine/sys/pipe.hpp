#ifndef IDIS_HELPERS_PIPE_HPP
#define IDIS_HELPERS_PIPE_HPP

#include "./fd.hpp"

#include "engine/error_handler/exception.hpp"

#include <unistd.h>
#include <cstring>
#include <memory>

namespace idis::sys
{
	class pipe
	{
	public:
		pipe()
		{
			constexpr size_t pipe_read_index  = 0;
			constexpr size_t pipe_write_index = 1;

			int fds[2];
			if(::pipe(fds) == -1) { throw exception{"create a pipe", strerror(errno)}; }

			m_read_fd  = fd_handle{fds[pipe_read_index]};
			m_write_fd = fd_handle{fds[pipe_write_index]};
		}

		void move_read_fd_to(int fd)
		{
			::dup2(m_read_fd.get(), fd);
			close_read_fd();
		}

		void close_read_fd() { m_read_fd.reset(); }

		void move_write_fd_to(int fd)
		{
			::dup2(m_write_fd.get(), fd);
			close_write_fd();
		}

		void close_write_fd() { m_write_fd.reset(); }

		auto read_fd() const { return m_read_fd.get(); }

		auto write_fd() const { return m_write_fd.get(); }

	private:
		fd_handle m_read_fd;
		fd_handle m_write_fd;
	};
}

#endif
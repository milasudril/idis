//@	{"dependencies_extra":[{"ref":"./child_proc.o","rel":"implementation"}]}

#ifndef IDIS_SYS_CHILDPROC_HPP
#define IDIS_SYS_CHILDPROC_HPP

#include "./pipe.hpp"
#include "engine/error_handler/exception.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstring>
#include <variant>
#include <vector>

namespace idis::sys
{
	class proc_exit_status
	{
	public:
		proc_exit_status() = default;

		explicit proc_exit_status(int value): m_value{value} {}

		auto value() const { return m_value; }

		bool operator==(proc_exit_status const&) const = default;
		bool operator!=(proc_exit_status const&) const = default;

	private:
		int m_value;
	};

	inline auto to_string(proc_exit_status val) { return std::to_string(val.value()); }

	inline auto has_error(proc_exit_status val) { return val.value() != 0; }

	class proc_term_signal
	{
	public:
		proc_term_signal() = default;

		explicit proc_term_signal(int value): m_value{value} {}

		auto value() const { return m_value; }

		bool operator==(proc_term_signal const&) const = default;
		bool operator!=(proc_term_signal const&) const = default;

	private:
		int m_value;
	};

	std::string to_string(proc_term_signal val);

	inline auto has_error(proc_term_signal) { return true; }

	inline std::variant<proc_exit_status, proc_term_signal> extract_proc_wait_status(int value)
	{
		if(WIFEXITED(value)) [[likely]] { return proc_exit_status{WEXITSTATUS(value)}; }
		else
		{
			return proc_term_signal{WTERMSIG(value)};
		}
	}

	class proc_wait_status
	{
	public:
		proc_wait_status() = default;

		explicit proc_wait_status(proc_term_signal val): m_value{val} {}
		explicit proc_wait_status(proc_exit_status val): m_value{val} {}

		explicit proc_wait_status(int value): m_value{extract_proc_wait_status(value)} {}

		auto value() const { return m_value; }

		bool operator==(proc_wait_status const&) const = default;
		bool operator!=(proc_wait_status const&) const = default;

	private:
		std::variant<proc_exit_status, proc_term_signal> m_value;
	};

	inline auto to_string(proc_wait_status status)
	{
		return std::visit(
		    [](auto value)
		    {
			    using std::to_string;
			    return to_string(value);
		    },
		    status.value());
	}

	inline auto has_error(proc_wait_status status)
	{
		return std::visit([](auto value) { return has_error(value); }, status.value());
	}

	class process_result
	{
	public:
		process_result() = default;

		explicit process_result(proc_wait_status status, std::vector<std::byte>&& error_log)
		    : m_status{status}
		    , m_stderr{std::move(error_log)}
		{
		}

		auto status() const { return m_status; }

		auto const& error_log() const { return m_stderr; }

	private:
		proc_wait_status m_status;
		std::vector<std::byte> m_stderr;
	};

	inline auto has_error(process_result const& res) { return has_error(res.status()); }

	class child_proc
	{
	public:
		template<class Function, class... Args>
		explicit child_proc(std::string&& name, Function&& f, Args&&... args)
		{
			auto pid = fork();
			if(pid == -1)
			{
				throw exception{std::string{"start "}.append(std::move(name)), strerror(errno)};
			}

			if(pid == 0)
			{
				m_pid = 0;
				m_stderr.close_read_fd();
				m_stderr.move_write_fd_to(STDERR_FILENO);
				int retval{};
				try
				{
					retval = f(std::forward<Args>(args)...);
				}
				catch(std::exception const& e)
				{
					fprintf(stderr, "%s: %s", name.c_str(), e.what());
					retval = -1;
				}
				catch(...)
				{
					fprintf(stderr, "%s: Unknown exception", name.c_str());
					retval = -1;
				}
				m_stderr.close_write_fd();
				_Exit(retval);
			}
			else
			{
				m_stderr.close_write_fd();
			}
			m_pid = pid;
		}

		~child_proc()
		{
			if(m_pid == -1) { return; }

			int wstatus{};
			waitpid(m_pid, &wstatus, 0);
		}

		process_result const& get_result() const
		{
			if(m_pid == -1) { return m_res; }

			fetch_result();
			return m_res;
		}

		auto pid() const { return m_pid; }

	private:
		pipe m_stderr;
		int m_exit_status;
		mutable pid_t m_pid;
		mutable process_result m_res;

		void fetch_result() const;
	};
}

#endif
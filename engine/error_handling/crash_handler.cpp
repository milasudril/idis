//@	{"target":{"name":"crash_handler.o"}}

#include "./crash_handler.hpp"

#include <signal.h>
#include <unistd.h>

#include <span>

namespace
{
	void write(int fd, std::span<std::byte const> buffer)
	{
		auto n   = std::size(buffer);
		auto ptr = std::data(buffer);
		while(n != 0)
		{
			auto n_written = ::write(fd, ptr, n);
			ptr += n_written;
			n -= n_written;
		}
	}

	template<class T>
	std::span<T const> as_bytes(T const&& obj) = delete;

	template<class T>
	auto as_bytes(T const& obj)
	{
		return std::span{reinterpret_cast<std::byte const*>(&obj), sizeof(T)};
	}

	void signal_handler(siginfo_t const& siginfo, ucontext_t const& ctxt)
	{
		constexpr std::array<std::byte const, 1> nulstring{static_cast<std::byte>(0x00)};
		write(STDERR_FILENO, nulstring);
		write(STDERR_FILENO, as_bytes(siginfo));
		write(STDERR_FILENO, as_bytes(ctxt));
		fsync(STDERR_FILENO);
		kill(getpid(), siginfo.si_signo);
	}
}

void idis::crash_handler::arm()
{
	struct sigaction sigact
	{
	};
	sigact.sa_flags = SA_SIGINFO | SA_RESETHAND;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_sigaction = [](int, siginfo_t* siginfo, void* uctxt)
	{ signal_handler(*siginfo, *reinterpret_cast<ucontext_t*>(uctxt)); };
	sigaction(SIGABRT, &sigact, nullptr);
	sigaction(SIGSEGV, &sigact, nullptr);
	sigaction(SIGBUS, &sigact, nullptr);
	sigaction(SIGILL, &sigact, nullptr);
	sigaction(SIGFPE, &sigact, nullptr);
	sigaction(SIGSYS, &sigact, nullptr);
	sigaction(SIGXFSZ, &sigact, nullptr);
	sigaction(SIGXCPU, &sigact, nullptr);
}

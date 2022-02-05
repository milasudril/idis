//@	{"target":{"name":"child_proc.o"}}

#include "./child_proc.hpp"

#define CASE_RET_STR(value)                                                                        \
	case value: return #value

std::string idis::sys::to_string(proc_term_signal val)
{
	switch(val.value())
	{
		CASE_RET_STR(SIGABRT);
		CASE_RET_STR(SIGBUS);
		CASE_RET_STR(SIGILL);
		CASE_RET_STR(SIGSEGV);
		CASE_RET_STR(SIGXCPU);
		CASE_RET_STR(SIGXFSZ);
		CASE_RET_STR(SIGFPE);
		CASE_RET_STR(SIGSYS);

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

std::string idis::sys::to_string(signal_cause val)
{
	switch(val.value())
	{
		CASE_RET_STR(SI_USER);
		CASE_RET_STR(SI_KERNEL);
		CASE_RET_STR(SI_QUEUE);
		CASE_RET_STR(SI_TIMER);
		CASE_RET_STR(SI_MESGQ);
		CASE_RET_STR(SI_ASYNCIO);
		CASE_RET_STR(SI_SIGIO);
		CASE_RET_STR(SI_TKILL);
	}

	switch(val.signo().value())
	{
		case SIGILL:
			switch(val.value())
			{
				CASE_RET_STR(ILL_ILLOPC);
				CASE_RET_STR(ILL_ILLOPN);
				CASE_RET_STR(ILL_ILLADR);
				CASE_RET_STR(ILL_ILLTRP);
				CASE_RET_STR(ILL_PRVOPC);
				CASE_RET_STR(ILL_PRVREG);
				CASE_RET_STR(ILL_COPROC);
				CASE_RET_STR(ILL_BADSTK);
			}
			break;

		case SIGFPE:
			switch(val.value())
			{
				CASE_RET_STR(FPE_INTDIV);
				CASE_RET_STR(FPE_INTOVF);
				CASE_RET_STR(FPE_FLTDIV);
				CASE_RET_STR(FPE_FLTOVF);
				CASE_RET_STR(FPE_FLTUND);
				CASE_RET_STR(FPE_FLTRES);
				CASE_RET_STR(FPE_FLTINV);
				CASE_RET_STR(FPE_FLTSUB);
			}
			break;

		case SIGBUS:
			switch(val.value())
			{
				CASE_RET_STR(BUS_ADRALN);
				CASE_RET_STR(BUS_ADRERR);
				CASE_RET_STR(BUS_OBJERR);
				CASE_RET_STR(BUS_MCEERR_AR);
				CASE_RET_STR(BUS_MCEERR_AO);
			}
			break;

		case SIGTRAP:
			switch(val.value())
			{
				CASE_RET_STR(TRAP_BRKPT);
				CASE_RET_STR(TRAP_TRACE);
				CASE_RET_STR(TRAP_BRANCH);
				CASE_RET_STR(TRAP_HWBKPT);
			}
			break;
	}

	return "<UNKNOWN>";
}

std::string idis::sys::to_string(signal_info const& info)
{
	auto& data = info.data;
	auto signo = proc_term_signal{data.si_signo};
	return std::string{"signo="}
	    .append(to_string(signo))
	    .append(" code=")
	    .append(to_string(signal_cause{data.si_code, signo}))
	    .append(" addr=")
	    // TODO: Use hex format
	    .append(std::to_string(reinterpret_cast<intptr_t>(data.si_addr)));
}

std::string idis::sys::to_string(user_context const& ctxt)
{
	auto& cpu_state = ctxt.data.uc_mcontext;
	// TODO: Use hex format
	return std::string{"rax="}
	    .append(std::to_string(cpu_state.gregs[REG_RAX]))
	    .append(" rbx=")
	    .append(std::to_string(cpu_state.gregs[REG_RBX]))
	    .append(" rcx=")
	    .append(std::to_string(cpu_state.gregs[REG_RCX]))
	    .append(" rdx=")
	    .append(std::to_string(cpu_state.gregs[REG_RDX]))
	    .append("\nr08=")
	    .append(std::to_string(cpu_state.gregs[REG_R8]))
	    .append(" r09=")
	    .append(std::to_string(cpu_state.gregs[REG_R9]))
	    .append(" r10=")
	    .append(std::to_string(cpu_state.gregs[REG_R10]))
	    .append(" r11=")
	    .append(std::to_string(cpu_state.gregs[REG_R11]))
	    .append("\nr12=")
	    .append(std::to_string(cpu_state.gregs[REG_R12]))
	    .append(" r13=")
	    .append(std::to_string(cpu_state.gregs[REG_R13]))
	    .append(" r14=")
	    .append(std::to_string(cpu_state.gregs[REG_R14]))
	    .append(" r15=")
	    .append(std::to_string(cpu_state.gregs[REG_R15]))
	    .append("\nrdi=")
	    .append(std::to_string(cpu_state.gregs[REG_RDI]))
	    .append(" rsi=")
	    .append(std::to_string(cpu_state.gregs[REG_RSI]))
	    .append(" rbp=")
	    .append(std::to_string(cpu_state.gregs[REG_RBP]))
	    .append(" rsp=")
	    .append(std::to_string(cpu_state.gregs[REG_RSP]))
	    .append("\nrip=")
	    .append(std::to_string(cpu_state.gregs[REG_RIP]))
	    .append(" rflags=")
	    .append(std::to_string(cpu_state.gregs[REG_EFL]))
	    .append(" cr2=")
	    .append(std::to_string(cpu_state.gregs[REG_CR2]))
	    .append(" seg=")
	    .append(std::to_string(cpu_state.gregs[REG_CSGSFS]));
}

std::string idis::sys::get_error_message(process_result const& res)
{
	std::string ret{};
	auto const& data = res.error_log();
	auto ptr         = std::data(data);
	auto const end   = ptr + std::size(data);
	while(ptr != end && *ptr != static_cast<std::byte>(0))
	{
		ret += static_cast<char>(*ptr);
		++ptr;
	}

	if(ptr == end) { return ret; }

	++ptr;
	if(static_cast<size_t>(end - ptr) < sizeof(siginfo_t)) { return ret; }

	{
		signal_info extended_info{};
		memcpy(&extended_info, ptr, sizeof(extended_info));
		ptr += sizeof(extended_info);
		ret.append("\n").append(to_string(extended_info)).append("\n");
	}

	if(static_cast<size_t>(end - ptr) < sizeof(ucontext_t)) { return ret; }

	{
		user_context extended_info{};
		memcpy(&extended_info, ptr, sizeof(extended_info));
		ret.append("\n").append(to_string(extended_info));
	}

	return ret;
}

std::string idis::sys::to_string(process_result const& res)
{
	if(has_signal(res.status()))
	{
		return std::string{"The process was terminated by a "}
		    .append(to_string(res.status()))
		    .append("\n\n")
		    .append(get_error_message(res));
	}
	else
	{
		return std::string{"The process returned "}
		    .append(to_string(res.status()))
		    .append("\n\n")
		    .append(get_error_message(res));
	}
}
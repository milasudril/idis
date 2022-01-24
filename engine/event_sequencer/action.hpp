#ifndef IDIS_EVENTSEQUENCER_ACTION_HPP
#define IDIS_EVENTSEQUENCER_ACTION_HPP

#include <utility>

namespace idis::seq
{
	class abstract_action
	{
	public:
		virtual void operator()()  = 0;
		virtual ~abstract_action() = default;
	};

	template<class T>
	class action: public abstract_action
	{
	public:
		explicit action(T&& obj): m_obj{std::move(obj)} {}

		void operator()() override { m_obj(); }

	private:
		T m_obj;
	};
}

#endif

#ifndef IDIS_EVENTDISPATCHER_EVENT_HPP
#define IDIS_EVENTDISPATCHER_EVENT_HPP

#include "./timestamp.hpp"

namespace idis::seq
{
	class Event
	{
	public:
		template<class T>
		explicit Event(Timestamp expires, T&& data): m_expires{expires}
		{
			m_data     = new T{std::move(data)};
			m_callback = [](void const* data, Timestamp t)
			{
				auto& obj = *static_cast<T const*>(data);
				obj(t);
			};
			m_dtor = [](void* obj)
			{
				auto obj = static_cast<T*>(data);
				delete obj;
			}
		}

		Event(Event const&) = delete;

		Event(Event&& other) noexcept
		{
			m_expires  = other.m_expires;
			m_data     = other.m_data;
			m_callback = std::exchange(other.m_callback,
			                           [](void*, Timestamp) {
				                           throw exception{"fire event", "No callback"};
			                           });
			m_dtor     = std::exchange(other.m_dtor, [](void*) {});
		}

		Event& operator=(Event const&) = delete;

		Event& operator=(Event&& other) noexcept
		{
			m_dtor(m_data);
			m_expires  = other.m_expires;
			m_data     = other.m_data;
			m_callback = std::exchange(other.m_callback,
			                           [](void*, Timestamp) {
				                           throw exception{"fire event", "No callback"};
			                           });
			m_dtor     = std::exchange(other.m_dtor, [](void*) {});
		}

		~Event() noexcept { m_dtor(m_data); }

		void operator()(Timestamp t) const { m_callback(m_data, t); }

	private:
		Timestamp m_expires;
		void* m_data;
		void (*m_callback)(void const*, Timestamp);
		void (*m_dtor)(void*);
	};
}

#endif
#ifndef IDIS_HELPERS_SORTEDSEQUENCE_HPP
#define IDIS_HELPERS_SORTEDSEQUENCE_HPP

#include <algorithm>

namespace idis
{
	template<class T,
	         class Container = std::vector<T>,
	         class Compare   = std::less<typename Container::value_type>>
	class sorted_sequence
	{
	public:
		using container_type = Container;
		using value_compare  = Compare;
		using value_type     = Container::value_type;
		using size_type      = Container::size_type;
		using const_iterator = Container::const_iterator;

		template<class First, class ... Args>
		requires(!std::is_same_v<std::decay_t<First>, sorted_sequence>)
		explicit sorted_sequence(First&& first, Args&&... args)
		    : m_container{std::forward<First>(first), std::forward<Args>(args)...}
		{
			std::ranges::sort(m_container, Compare{});
		}

		decltype(auto) begin() const { return std::begin(m_container); }

		decltype(auto) end() const { return std::end(m_container); }

		decltype(auto) size() const { return std::size(m_container); }

		decltype(auto) operator[](size_t index) const { return m_container[index]; }

		auto operator<=>(sorted_sequence const&) const = default;

	private:
		Container m_container;
	};

	template<class T,
	         class Container = std::vector<T>,
	         class Compare   = std::less<typename Container::value_type>>
	class sorted_view
	{
	public:
		using container_type = Container;
		using value_compare  = Compare;
		using value_type     = Container::value_type;
		using size_type      = Container::size_type;
		using const_iterator = Container::const_iterator;

		sorted_view(std::reference_wrapper<sorted_sequence<T, Container, Compare>> src)
		    : m_begin{std::begin(src.get())}
		    , m_end{std::end(src.get())}
		{
		}

		auto begin() const { return m_begin; }

		auto end() const { return m_end; }

		auto size() const { return m_end - m_begin; }

		auto const& operator[](size_t index) const { return m_begin[index]; }

	private:
		const_iterator m_begin;
		const_iterator m_end;
	};
}

#endif
#ifndef IDIS_HELPERS_INTEGERMIXIN_HPP
#define IDIS_HELPERS_INTEGERMIXIN_HPP

namespace idis
{
	template<class Rep, class UserClass>
	class integer_mixin
	{
	public:
		using rep = Rep;

		constexpr explicit integer_mixin(rep val): m_value{val} {}

		constexpr auto value() const { return m_value; }

		static constexpr auto zero() { return static_cast<UserClass>(integer_mixin{0}); }

		constexpr auto operator<=>(integer_mixin const& other) const = default;

		constexpr auto& operator++()
		{
			++m_value;
			return static_cast<UserClass&>(*this);
		}

		constexpr auto operator++(int)
		{
			auto tmp = *this;
			++m_value;
			return UserClass{tmp.value()};
		}

		constexpr auto& operator--()
		{
			--m_value;
			return static_cast<UserClass&>(*this);
		}

		constexpr auto operator--(int)
		{
			auto tmp = *this;
			--m_value;
			return UserClass{tmp.value()};
		}

		constexpr auto& operator+=(UserClass other)
		{
			m_value += other.m_value;
			return static_cast<UserClass&>(*this);
		}

		constexpr auto& operator-=(UserClass other)
		{
			m_value -= other.m_value;
			return static_cast<UserClass&>(*this);
		}

		constexpr auto& operator*=(rep value)
		{
			m_value *= value;
			return static_cast<UserClass&>(*this);
		}

		constexpr auto& operator/=(rep value)
		{
			m_value/=value;
			return static_cast<UserClass&>(*this);
		}


	private:
		rep m_value;
	};
}
#endif

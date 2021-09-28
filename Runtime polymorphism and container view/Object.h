#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <memory>
#include <assert.h>
#include <any>
#include <ranges>
#include <optional>
#include "SFML/Graphics/RenderWindow.hpp"


template<typename object_impl>
concept object_like = requires (object_impl obj, sf::RenderWindow& w) 
{
	{ obj.draw(w) } -> std::same_as<void>;
};

struct ibase 
{
	void(*draw)(std::any&, sf::RenderWindow&);
};

template<std::semiregular ConcreteType>
constexpr ibase vtable_for
{
	[](std::any& _storage, sf::RenderWindow& w) { std::any_cast<ConcreteType&>(_storage).draw(w); },
};

class Object
{
public:

	template<object_like ConcreteType>
	requires (!std::same_as<std::remove_cvref_t<ConcreteType>, Object>)
		Object(ConcreteType&& x) : m_storage(std::make_any<ConcreteType>(x)),
		m_vtable(&vtable_for<ConcreteType>) {}

	void draw(sf::RenderWindow&);

private:
	std::any m_storage{};
	std::shared_ptr<ibase const> m_vtable{};
};

//template<std::semiregular T>
//class maybe_view : public std::ranges::view_interface<maybe_view<T>>
//{
//public:
//	maybe_view() = default;
//
//	maybe_view(T x) : m_data(std::move(x)) {}
//
//	T const* begin() const noexcept {
//		return m_data ? &*m_data : nullptr;
//	}
//
//	T const* end() const noexcept {
//		return m_data ? &*m_data + 1 : nullptr;
//	}
//
//	void draw(sf::RenderWindow& w)
//	{
//		*this | std::views::transform(this->m_data.value().draw(w)) | std::views::join;
//	}
//
//	template<std::ranges::output_range TContainer>
//	constexpr auto to(TContainer const& c) 
//	{
//		auto x = *this | std::views::common;
//		return TContainer(x.begin(), x.end());
//	}
//private:
//	std::optional<T> m_data{};
//};

#endif


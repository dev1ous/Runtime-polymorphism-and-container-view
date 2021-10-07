#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <memory>
#include <assert.h>
#include <any>
#include <ranges>
#include <optional>
#include <iostream>
#include <type_traits>
#include "SFML/Graphics/RenderWindow.hpp"

template<class object_impl>
concept object_like = requires (object_impl obj, sf::RenderWindow& w) 
{
	{ obj.draw(w) } -> std::same_as<void>;
	requires std::semiregular<object_impl>;
};

struct Ibase 
{
	void(*draw)(std::any const&, sf::RenderWindow&);
};

template<class ConcreteType>
inline constexpr Ibase make_vtable
{
	[] (std::any const& _storage, sf::RenderWindow& w) { std::any_cast<ConcreteType const&>(_storage).draw(w); }
};

class Object
{
public:
	template<object_like ConcreteType>
	Object(ConcreteType&& x) : m_storage(std::forward<ConcreteType>(x)),
		m_vtable(std::make_shared<Ibase const>(make_vtable<ConcreteType>)) {}

	void draw(sf::RenderWindow&) const;

private:
	std::any m_storage{};
	std::shared_ptr<Ibase const> m_vtable{};
};

template<std::semiregular T>
struct view_container : public std::ranges::view_interface<view_container<T>>
{
	template<std::ranges::input_range R>
	requires std::same_as<T, std::ranges::range_value_t<R>>
		view_container(R&& r) : view_container(std::ranges::begin(r), std::ranges::end(r)) {}

	void draw(sf::RenderWindow& w)
	{
		*this | std::views::transform([](auto&& x) { return x.draw(w); }) | std::views::join;
	}
};

#endif


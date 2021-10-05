#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <memory>
#include <assert.h>
#include <any>
#include <ranges>
#include <optional>
#include <iostream>
#include "SFML/Graphics/RenderWindow.hpp"

template<typename object_impl>
concept object_like = requires (object_impl obj, sf::RenderWindow& w) 
{
	{ obj.draw(w) } -> std::same_as<void>;
	requires std::semiregular<object_impl>;
};

struct ibase 
{
	void(*draw)(std::any const&, sf::RenderWindow&);
};

template<typename ConcreteType>
inline constexpr ibase make_vtable
{
	[] (std::any const& _storage, sf::RenderWindow& w) { std::any_cast<ConcreteType const&>(_storage).draw(w); }
};

class Object
{
public:
	template<object_like ConcreteType>
	Object(ConcreteType&& x) : m_storage(std::forward<ConcreteType>(x)),
		m_vtable(std::make_shared<ibase const>(make_vtable<ConcreteType>)) {}

	void draw(sf::RenderWindow&) const;

private:
	std::any m_storage{};
	std::shared_ptr<ibase const> m_vtable{};
};

template<std::semiregular T>
class view_container : public std::ranges::view_interface<view_container<T>>
{
public:
	view_container() = default;

	template<template<typename> class TContainer>
	requires std::ranges::input_range<TContainer>
		view_container(TContainer<T>const& x) : std::ranges::cbegin(x.begin())
		std::ranges::end(x.end) {}

	void draw(sf::RenderWindow& w)
	{
		*this | std::views::transform([](auto&& x) { return x.draw(w); }) | std::views::join;
	}
};

#endif


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

template<typename T>
concept is_mapped = requires(T t)
{
	typename T::key_type;
	typename T::mapped_type;
	typename T::value_type;
	typename T::iterator;
	requires std::same_as<decltype(t.begin()), typename T::iterator>;
	requires std::same_as<decltype(t.end()), typename T::iterator>;
	requires std::same_as<typename T::value_type, std::iter_value_t<typename T::iterator>>;
	requires std::same_as<typename T::value_type, std::pair<const typename T::key_type, typename T::mapped_type>>;
};

template<template<class...> class TContainer, class ...Args >
using has_container_t = std::conditional_t < sizeof...(Args) == 1, std::ranges::input_range<TContainer>, is_mapped<TContainer>>;

template<class ...Args>
requires ((sizeof...(Args) > 0) && (sizeof...(Args) <= 2))
class view_container : public std::ranges::view_interface<view_container<Args...>>
{
public:

	template<template<class...> class TContainer>
	requires std::same_as<typename TContainer::value_type, std::ranges::range_value_t<TContainer>>
		view_container(has_container_t<TContainer<Args...>>const& x) : std::ranges::begin(x),
		std::ranges::end(x) {}  

	void draw(sf::RenderWindow& w)
	{
		*this | std::views::transform([](auto&& x) { return x.draw(w); }) | std::views::join;
	}
};

#endif


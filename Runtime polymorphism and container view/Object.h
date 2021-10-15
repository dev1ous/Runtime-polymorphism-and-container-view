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
	void(*draw)(std::any&, sf::RenderWindow&);
	std::any(*copy)(std::any const&);
	std::any(*move)(std::any&);
};

template<class ConcreteType>
inline constexpr Ibase make_vtable
{
	[](std::any& _storage, sf::RenderWindow& w) { std::any_cast<ConcreteType&>(_storage).draw(w); },
	[](std::any const& _storage) -> std::any { return std::any_cast<ConcreteType const&>(_storage); },
	[](std::any& _storage) -> std::any { return std::move(std::any_cast<ConcreteType&>(_storage)); }
};

class Object
{
public:
	template<object_like ConcreteType>
	requires (!std::same_as<std::remove_cvref_t<ConcreteType>, Object>)
		Object(ConcreteType&& x) : m_storage(std::forward<ConcreteType>(x)),
		m_vtable(std::make_shared<Ibase const>(make_vtable<ConcreteType>)) {}
	~Object() = default;

	Object(Object const&);
	Object& operator=(Object const&);
	Object(Object&&) noexcept;
	Object& operator=(Object&&) noexcept;

	void draw(sf::RenderWindow&);

private:
	std::any m_storage{};
	std::shared_ptr<Ibase const> m_vtable{};
};

template<std::ranges::input_range R>
requires std::ranges::view<R>
class view_container : public std::ranges::view_interface<view_container<R>>
{
public:
	constexpr view_container() noexcept = default;
	constexpr view_container(R&& r) noexcept : m_data(std::move(r)) {}

	[[nodiscard]] constexpr std::ranges::iterator_t<R> begin() const noexcept(
		noexcept(std::ranges::begin(m_data.value()))) {
		return std::ranges::begin(m_data.value());
	}
	[[nodiscard]] constexpr std::ranges::sentinel_t<R> end() const noexcept(
		noexcept(std::ranges::end(m_data.value()))) {
		return std::ranges::end(m_data.value());
	}

	/*void draw(sf::RenderWindow& w) {
		*this | std::views::transform([&w](auto&& x) { x.draw(w); });
	}*/
private:
	std::optional<std::reference_wrapper<R>> m_data{};
};

template<std::ranges::input_range R>
view_container(R&&)->view_container<std::views::all_t<R>>;

template <std::ranges::range R>
inline constexpr bool std::ranges::enable_borrowed_range<view_container<R>> = true;

#endif


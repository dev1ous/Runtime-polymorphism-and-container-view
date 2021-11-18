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
#include "SFML/Graphics.hpp"

class my_class {
public:

	my_class() {
		m.loadFromFile("../Ressources/golden.png");
		t.setTexture(m);
		t.setPosition(0.f, 0.f);
	}
	void draw(sf::RenderWindow& w) const {
		w.draw(t);
	}
	sf::Sprite t;
	sf::Texture m;
};

struct Draw {
	void operator()(my_class const& mc, sf::RenderWindow& w) {
		mc.draw(w);
	}
};

template<class object_impl>
concept object_like = requires (object_impl obj, sf::RenderWindow& w) 
{
	{ obj.draw(w) } -> std::same_as<void>;
};

struct IBase
{
	void(*accept)(std::any&, std::any const&);
	void(*draw)(std::any&, sf::RenderWindow&);
};

template<class ConcreteType>
inline constexpr IBase make_vtable
{
	[](std::any& storage, std::any const& visitor) { std::any_cast<ConcreteType&>(storage); },
	[](std::any& storage, sf::RenderWindow& w) { std::any_cast<ConcreteType&>(storage).draw(w); }
};

class Object
{
public:
	template<object_like ConcreteType>
	requires (not std::same_as<std::remove_cvref_t<ConcreteType>, Object>) and (std::semiregular<ConcreteType>)
		constexpr Object(ConcreteType&& x) noexcept : m_storage(std::forward<ConcreteType>(x)),
		m_vtable(std::make_shared<IBase const>(make_vtable<ConcreteType>)) {}

	void accept(auto const& visit);

	void draw(sf::RenderWindow& w) {
		m_vtable->draw(m_storage, w);
	}

private:
	std::any m_storage{};
	std::shared_ptr<IBase const> m_vtable{};
};

template<std::ranges::range R>
requires std::ranges::view<R>
class view_container : public std::ranges::view_interface<view_container<R>>
{
public:
	constexpr view_container() noexcept = default;
	constexpr view_container(R&& r) noexcept : m_data(std::move(r)) {}

	R const* begin() const noexcept {
		return m_data.has_value() ? std::addressof(m_data.value()) : nullptr;
	}

	R const* end() const noexcept {
		return m_data.has_value() ? std::addressof(m_data.value()) + 1 : nullptr;
	}

private:
	std::optional<R> m_data{};
};

//template<std::ranges::input_range R>
//view_container(R&)->view_container<R>;

template<std::ranges::range R>
view_container(R&&)->view_container<std::views::all_t<R>>;

template <std::ranges::input_range R>
inline constexpr bool std::ranges::enable_borrowed_range<view_container<R>> = true;

void draw(std::ranges::range auto& v, sf::RenderWindow& w)
{
	auto i = v | std::views::transform([](auto underlying_view) { return underlying_view; }) | std::views::join
		| std::views::transform([&w](auto x)->decltype(x) { x.draw(w); });
}

inline constexpr auto yield_if =
[]<std::semiregular T>(bool b, T x) {
	return b ? view_container{ std::move(x) }
	: view_container<T>{};
};
#endif

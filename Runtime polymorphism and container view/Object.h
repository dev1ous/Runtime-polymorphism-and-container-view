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

struct Object
{
	Object(object_like auto&& x) : m_storage(std::forward<decltype(x)>(x)),
		m_vtable(std::make_shared<Ibase const>(make_vtable<decltype(x)>)) {}

	void draw(sf::RenderWindow&) const;

private:
	std::any m_storage{};
	std::shared_ptr<Ibase const> m_vtable{};
};

template<std::ranges::range R>
requires std::ranges::view<R>
class view_container : public std::ranges::view_interface<view_container<R>>
{
public:
	view_container() = default;
	view_container(R&& r) :  m_data(std::move(r)){}

	constexpr decltype(auto) begin() const noexcept {
		return std::ranges::cbegin(m_data.value());
	}
	constexpr decltype(auto) end() const noexcept {
		return std::ranges::cend(m_data.value());
	}

	/*void draw(sf::RenderWindow& w) {
		*this | std::views::transform([&w](auto&& x) { x.draw(w); });
	}*/
private:
	std::optional<R> m_data{};
};

template<std::ranges::range R>
view_container(R&&)->view_container<std::views::all_t<R>>;

#endif


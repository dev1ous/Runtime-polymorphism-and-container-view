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
};

struct IDrawStrategy
{
	void(*draw)(std::any&, sf::RenderWindow&);
};

template<class ConcreteType>
inline constexpr IDrawStrategy make_vtable
{
	[](std::any& _storage, sf::RenderWindow& w) { std::any_cast<ConcreteType&>(_storage).draw(w); }
};

template<class Interface>
class interface_impl
{
public:
	template<object_like ConcreteType>
	requires (!std::same_as<std::remove_cvref_t<ConcreteType>, interface_impl>) && (std::semiregular<ConcreteType>)
		interface_impl(ConcreteType&& x) : m_storage(std::forward<ConcreteType>(x)),
		m_vtable(std::make_shared<Interface const>(make_vtable<ConcreteType>)) {}

private:
	std::any m_storage{};
	std::shared_ptr<Interface const> m_vtable{};
};

namespace obj 
{
	struct draw_fn
	{
	private:
		constexpr auto operator()(auto& x, sf::RenderWindow& w)
		{
			x.m_vtable->draw(x.m_storage, w);
		}
	};
	inline constexpr draw_fn draw{};
}

template<std::ranges::range R>
requires std::ranges::view<R>
class view_container : public std::ranges::view_interface<view_container<R>>
{
public:
	constexpr view_container() noexcept = default;
	constexpr view_container(R&& r) noexcept : m_data(std::move(r)) {}

	R const* begin() const noexcept {
		return m_data ? &*m_data : nullptr;
	}

	R const* end() const noexcept {
		return m_data ? &*m_data + 1 : nullptr;
	}
	void draw(sf::RenderWindow& w)
	{
		auto i = *this | std::views::transform([](auto underlying_view) { return underlying_view; }) | std::views::join
			| std::views::transform([&w](auto& x) -> decltype(x) { obj::draw(x, w); });
	}
private:
	std::optional<R> m_data{};
};

template<std::ranges::range R>
view_container(R&&)->view_container<std::views::all_t<R>>;

template <std::ranges::range R>
inline constexpr bool std::ranges::enable_borrowed_range<view_container<R>> = true;

#endif

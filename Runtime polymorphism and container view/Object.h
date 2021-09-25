#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <memory>
#include <assert.h>
#include <any>
#include <ranges>
#include "SFML/Graphics/RenderWindow.hpp"

template<typename T, typename U>
concept copy_constructor_for_ref = !std::same_as<std::remove_cvref_t<T>, U>;

struct ibase {
	void(*draw)(std::any&);
};

template<typename ConcreteType>
constexpr ibase vtable_for{
	[](std::any& _storage) { std::any_cast<ConcreteType&>(_storage).draw(std::add_lvalue_reference<sf::RenderWindow>{}); },
};

class Object
{
public:
	Object(copy_constructor_for_ref<Object> auto&& x) : m_storage{ std::make_any(x) },
		m_vtable{ std::move<decltype(x)>(vtable_for),[](std::any& _storage)
			{delete std::any_cast<std::remove_cvref_t<decltype(x)>>(&_storage); } } {}

	void draw(sf::RenderWindow&);

private:
	std::any m_storage;
	std::shared_ptr<ibase const> m_vtable;
};

template<typename T>
class composite : public std::ranges::view_interface<composite<T>> {
public:

	composite(std::ranges::range auto const& x) : m_begin{ std::ranges::begin(x) },
		m_end{ std::ranges::end(x) }{}

	auto begin()const noexcept {
		return m_begin;
	}

	auto end()const noexcept {
		return m_end;
	}

	/*void draw(sf::RenderWindow& w) const {
		if constexpr (std::same_as < T, typename TContainer::mapped_type>) {
			for (auto&& [k, v] : *this) {
				!:m

			}
		}
	}*/
private:
	typename std::ranges::iterator_t<T> m_begin;
	typename std::ranges::sentinel_t<T> m_end;
};
#endif


#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <memory>
#include <assert.h>
#include <any>
#include <ranges>
#include <optional>
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

struct Object
{
	Object(copy_constructor_for_ref<Object> auto&& x) : m_storage(std::make_any(x)),
		m_vtable(std::move<decltype(x)>(vtable_for), [](std::any& _storage)
			{delete std::any_cast<std::remove_cvref_t<decltype(x)>>(&_storage); }) {}

	void draw(sf::RenderWindow&);

private:
	std::any m_storage{};
	std::shared_ptr<ibase const> m_vtable{};
};

template<std::semiregular T>
struct composite : std::ranges::view_interface<composite<T>> {
	composite() = default;

	composite(T x) : m_data(std::move(x)) {}

	T const* begin() const noexcept {
		return m_data ? &*m_data : nullptr;
	}
	T const* end() const noexcept {
		return m_data ? &*m_data + 1 : nullptr;
	}

private:
	std::optional<T> m_data{};
};
#endif


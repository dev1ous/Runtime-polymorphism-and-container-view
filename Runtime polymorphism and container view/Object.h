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
	void(*destroy)(std::any const&);
	std::any&(*clone)(std::any&);
	std::any&(*move_clone)(std::any&);
	void(*draw)(std::any&, sf::RenderWindow const&);
};

template<typename ConcreteType>
constexpr ibase vtable_def
{
	[] (std::any const& _storage) {delete std::any_cast<ConcreteType>(&_storage); },
	[] (std::any& _storage) {std::make_any<ConcreteType>(std::any_cast<ConcreteType&>(_storage)); },
	[] (std::any& _storage) {std::make_any<ConcreteType>(std::move(std::any_cast<ConcreteType&>(_storage))); },
	[] (std::any const& _storage, sf::RenderWindow& w) { std::any_cast<ConcreteType&>(_storage).draw(w); }
};

class Object
{
public:

	template<object_like ConcreteType>
	requires (!std::same_as<std::remove_cvref_t<ConcreteType>, Object>) && std::semiregular<ConcreteType>
		Object(ConcreteType&& x) : m_storage(std::make_any<ConcreteType>(x)),
		m_vtable(std::make_shared<ibase const>(vtable_def<ConcreteType>)) {}
	~Object();

	Object(Object const&);
	Object(Object&&) noexcept;
	Object& operator=(Object const&);
	Object& operator=(Object&&) noexcept;

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
//  template<std::ranges::output_range TContainer>
//	maybe_view(TContainer<T> x) :  {}
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
//		*this | std::views::transform([](auto&& x){ return x.draw(w) }) | std::views::join;
//	}
//
//private:
//	std::optional<T> m_data{};
//};

#endif


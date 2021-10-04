#include "Object.h"

Object::~Object()
{
	m_vtable->destroy(m_storage);
}

Object::Object(Object const& x) :m_storage(x.m_vtable->clone(x.m_storage)),
m_vtable(x.m_vtable){}

Object::Object(Object&& x) noexcept : m_storage(x.m_vtable->move_clone(x.m_storage)),
m_vtable(std::move(x.m_vtable)) {}


Object& Object::operator=(Object const& x)
{
	m_storage = x.m_vtable->clone(x.m_storage);
	m_vtable = x.m_vtable;
	return *this;
}

Object& Object::operator=(Object&& x) noexcept
{
	m_storage = x.m_vtable->move_clone(x.m_storage);
	m_vtable = std::move(x.m_vtable);
	return *this;
}


void Object::draw(sf::RenderWindow& w)
{
	m_vtable->draw(m_storage, w);
}




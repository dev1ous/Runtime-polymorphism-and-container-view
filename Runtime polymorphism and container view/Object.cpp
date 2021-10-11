#include "Object.h"


Object::Object(Object const& x) :
	m_storage(x.m_vtable->copy(x.m_storage)), m_vtable(x.m_vtable) {}

Object& Object::operator=(Object const& x)
{
	m_storage = x.m_vtable->copy(x.m_storage);
	m_vtable = x.m_vtable;
	return *this;
}

Object::Object(Object&& x) noexcept:
	m_storage(x.m_vtable->move(x.m_storage)), m_vtable(x.m_vtable) {}

Object& Object::operator=(Object&& x) noexcept
{
	m_storage = x.m_vtable->move(x.m_storage);
	m_vtable = x.m_vtable;
	return *this;
}

void Object::draw(sf::RenderWindow& w)
{
	m_vtable->draw(m_storage, w);
}




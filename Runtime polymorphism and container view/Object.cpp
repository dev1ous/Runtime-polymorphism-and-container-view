#include "Object.h"

void Object::draw(sf::RenderWindow& w) const
{
	m_vtable->draw(m_storage, w);
}




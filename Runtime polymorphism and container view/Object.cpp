#include "Object.h"

void Object::draw(sf::RenderWindow& w)
{
	m_vtable->draw(m_storage, w);
}



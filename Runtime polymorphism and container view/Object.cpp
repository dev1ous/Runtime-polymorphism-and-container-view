#include "Object.h"

void Object::accept(auto const& visit)
{
	m_vtable->accept(m_storage, std::any(visit));
}

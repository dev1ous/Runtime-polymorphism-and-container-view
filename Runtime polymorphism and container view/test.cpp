#include "Object.h"

class my_class {

};

auto main()->int
{
	std::vector<Object> x;
	x.push_back(my_class{});

	return 0;
}

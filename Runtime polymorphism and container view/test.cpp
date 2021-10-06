#include "Object.h"
#include "SFML/Graphics.hpp"
#include <list>

class my_class {
public:

	my_class() {

	}
	void draw(sf::RenderWindow& w) const{
		w.draw(t);
	}
	sf::Sprite t;
	sf::Texture m;
};

int x[100];

auto main()->int
{
	sf::RenderWindow w;
	std::vector<Object> x;
	x.push_back(my_class{});
	view_container view{ x };

	for (auto&& g : x) {
		g.draw(w);
	}

	return 0;
}

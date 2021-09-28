#include "Object.h"
#include "SFML/Graphics.hpp"

class my_class {
public:

	my_class() {

	}
	void draw(sf::RenderWindow& w) {
		w.draw(t);
	}
	sf::Sprite t;
	sf::Texture m;
};

auto main()->int
{
	sf::RenderWindow w;
	std::vector<Object> x;
	x.push_back(my_class{});

	/*for (auto&& g : x) {
		g.draw(w);
	}*/

	return 0;
}

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

int main() {
	std::vector<Object> x;
	view_container v{ x };
	return 0;
}

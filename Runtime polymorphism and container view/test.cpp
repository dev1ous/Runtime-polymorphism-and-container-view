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

int main()
{
	sf::RenderWindow w;
	std::vector<interface_impl<IDrawStrategy>> x{ my_class{} };
	view_container view{ x };
	view.draw(w);

	return 0;
}

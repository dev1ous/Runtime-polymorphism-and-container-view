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
	sf::RenderWindow w;
	auto x = std::vector<Object>{ my_class{} };
	view_container v{ x };

	auto view = v | std::views::transform([&w](auto&& t) { t.draw(w); }) ;

	return 0;
}

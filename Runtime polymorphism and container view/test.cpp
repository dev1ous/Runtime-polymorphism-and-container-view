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

std::vector<Object> x{ my_class{} };
view_container view{ x };

auto main() -> int
{
	sf::RenderWindow w;

	auto i = view | std::views::transform([&w](auto&& obj) {obj.draw(w); });

	return 0;
}

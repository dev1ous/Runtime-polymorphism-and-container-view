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

auto main() -> int
{
	sf::RenderWindow w;
	std::vector<Object> x{ my_class{} };
	view_container view{ x };
	/*auto i = view | std::views::transform([&w](auto underlying_view) { return underlying_view 
		| std::views::transform([&w](auto& obj) -> decltype(obj) { obj.draw(w); }); }) | std::views::join;*/

	auto k = view | std::views::transform([](auto underlying_view) { return underlying_view; }) | std::views::join 
		| std::views::transform([&w](auto& obj) -> decltype(obj) { obj.draw(w); });
	return 0;
}

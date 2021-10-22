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

<<<<<<< Updated upstream
=======
	/*auto k = view | std::views::transform([](auto underlying_view) { return underlying_view; }) | std::views::join 
		| std::views::transform([&w](auto& obj) -> decltype(obj) { obj.draw(w); });*/
>>>>>>> Stashed changes
	return 0;
}

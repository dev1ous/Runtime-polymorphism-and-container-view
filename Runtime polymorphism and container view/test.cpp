#include "Object.h"
#include <numeric>

auto main() -> int
{
	sf::RenderWindow w({ 1920,1080 }, "");
	std::vector<Object> x{ my_class{} };
	view_container v{ x };

	while (w.isOpen())
	{

		for (sf::Event e = sf::Event{}; w.pollEvent(e);)
		{
			if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Key::Escape)
				w.close();
		}

		w.clear();
		/*auto rng = v | std::views::transform([](auto x) { return x; }) | std::views::join 
			| std::views::transform([&w](auto& obj)->decltype(obj) { obj.draw(w); });*/
		draw(x, w);

		/*std::ranges::for_each(view, [&w](auto x) {x.draw(w); })*/
		w.display();
	}


	return 0;
}

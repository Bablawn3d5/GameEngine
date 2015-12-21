#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <entityx/entityx.h>

//Quick test for EntityX
struct ComponentFoo : entityx::Component<ComponentFoo> {
    double num;
};

// Quick test for Box2d
b2CircleShape c;

int main()
{
   sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
   sf::CircleShape shape(100.f);
   shape.setFillColor(sf::Color::Green);

   while (window.isOpen())
   {
      sf::Event event;
      while (window.pollEvent(event))
      {
         if (event.type == sf::Event::Closed)
            window.close();
      }

      window.clear();
      window.draw(shape);
      window.display();
   }

   return 0;
}
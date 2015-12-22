// Copyright 2015 Bablawn3d5

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <entityx/entityx.h>
#include <Farquaad/Systems.hpp>

// Quick test for Box2d
b2CircleShape c;

// Quick test for EntityX
class Application : public entityx::EntityX {
public:
    explicit Application(sf::RenderWindow &target) {
        auto inputSystem = systems.add<InputSystem>(target);

        // TODO(SMA) : Load test binds from seralized list.
        // Add Test Binds
        inputSystem->bindEventToKeyPress("Move", sf::Keyboard::W);
        inputSystem->bindEventToKeyPress("Use", sf::Keyboard::E);

        systems.configure();
    }

    void update(ex::TimeDelta dt) {
        systems.update<InputSystem>(dt);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    Application app(window);

    sf::Clock clock;
    while ( window.isOpen() ) {
        window.clear();
        sf::Time elapsed = clock.restart();
        app.update(elapsed.asSeconds());
        window.draw(shape);
        window.display();
    }

    return 0;
}

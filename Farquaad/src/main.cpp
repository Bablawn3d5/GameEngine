// Copyright 2015 Bablawn3d5

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <entityx/entityx.h>
#include <Farquaad/Systems.hpp>
#include <Farquaad/Components.hpp>

// Quick test for Box2d
b2CircleShape c;

// TODO(SMA) : Remove me and replace me with Box2D Physics
// Updates a body's position by and arbirary set of units.
struct BodySystem : public ex::System<BodySystem> {
    void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
        es.each<Body, Stats>([dt](ex::Entity entity, Body &body, Stats& stat) {
            body.position += body.direction * static_cast<float>(stat.speed * dt);
        });
    };
};

// Quick test for EntityX
class Application : public entityx::EntityX {
public:
    explicit Application(sf::RenderWindow &target) {
        auto inputSystem = systems.add<InputSystem>(target);

        // TODO(SMA) : Load test binds from seralized list.
        // HACK(SMA) : Add Test Binds directly in applicaiton creation.
        inputSystem->bindEventToKeyPress("MoveUp", sf::Keyboard::W);
        inputSystem->bindEventToKeyPress("MoveDown", sf::Keyboard::S);
        inputSystem->bindEventToKeyPress("MoveLeft", sf::Keyboard::A);
        inputSystem->bindEventToKeyPress("MoveRight", sf::Keyboard::D);

        inputSystem->bindEventToKeyPress("Use", sf::Keyboard::E);

        systems.add<RenderSystem>(target);
        systems.add<BodySystem>();

        // HACK(SMA) : Create entity right in this bloated constructor.
        // TODO(SMA) : Replace me using seralized components.
        {
            auto entity = entities.create();
            entity.assign<Body>();
            entity.assign<Stats>(100.0f);
            entity.assign<RenderableShape>(new sf::RectangleShape({ 10,10 }));
            entity.assign<InputResponder>();
        }

        // HACK(SMA) : Create 'background' right up in here.
        {
            std::unique_ptr<sf::CircleShape> shape(new sf::CircleShape(100.f));
            shape->setFillColor(sf::Color::Green);

            auto entity = entities.create();
            entity.assign<Body>();
            entity.assign<RenderableShape>(std::move(shape));
        }
        systems.configure();
    }

    void update(ex::TimeDelta dt) {
        systems.update<InputSystem>(dt);
        systems.update<RenderSystem>(dt);
        systems.update<BodySystem>(dt);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    window.setKeyRepeatEnabled(false);

    Application app(window);

    sf::Clock clock;
    while ( window.isOpen() ) {
        window.clear();
        sf::Time elapsed = clock.restart();
        app.update(elapsed.asSeconds());
        window.display();
    }

    return 0;
}

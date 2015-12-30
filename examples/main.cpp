// Copyright 2015 Bablawn3d5

#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <entityx/entityx.h>
#include <Farquaad/Systems.hpp>
#include <Farquaad/Components.hpp>
#include <Farquaad/Core.hpp>
#include <Farquaad/Serialization.hpp>
#include <Farquaad/Thor/ResourceLoader.hpp>
#include <Farquaad/Box2D/SFMLB2DDebug.h>
#include <Thor/Resources.hpp>
#include <json/json.h>

// Quick test for Box2d
b2CircleShape c;

// Test for JSONCPP
Json::Value v;

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
    std::shared_ptr<b2World> physWorld;
    std::shared_ptr<SFMLB2DDebug> debugDraw;

    explicit Application(sf::RenderWindow &target, Json::Value& v) {
        b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
        physWorld = std::make_shared<b2World>(gravity);
        debugDraw = std::make_shared<SFMLB2DDebug>(target);
        debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit |
                            b2Draw::e_aabbBit | b2Draw::e_pairBit);

        systems.add<PhysicsSystem>(physWorld, debugDraw.get());
        auto inputSystem = systems.add<InputSystem>(target);
        inputSystem->setKeybinds(Serializable::fromJSON<InputSystem::KeyBindMap>(v["keys"]));

        systems.add<RenderSystem>(target);
        systems.add<BodySystem>();
        systems.configure();

        // HACK(SMA) : Create entity right in this bloated constructor.
        // TODO(SMA) : Replace me using seralized components.
        {
            auto entity = entities.create();
            entity.assign<Body>(20.0f, 20.0f);
            entity.assign<Stats>(100.0f);
            entity.assign<Physics>(b2_dynamicBody, 100, 100);
            entity.assign<InputResponder>();
        }

        {
            auto entity = entities.create();
            entity.assign<Body>(200.0f, 200.0f);
            entity.assign<Stats>(100.0f);
            entity.assign<Physics>(b2_staticBody, 100, 100);
        }

        // HACK(SMA) : Create 'background' right up in here.
        {
            std::unique_ptr<sf::CircleShape> shape(new sf::CircleShape(100.f));
            shape->setFillColor(sf::Color::Green);

            auto entity = entities.create();
            entity.assign<Body>();
            entity.assign<RenderableShape>(std::move(shape));
        }
    }

    void update(ex::TimeDelta dt) {
        systems.update<InputSystem>(dt);
        systems.update<BodySystem>(dt);
        systems.update<PhysicsSystem>(dt);
        systems.update<RenderSystem>(dt);
        physWorld->DrawDebugData();
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    window.setKeyRepeatEnabled(false);

    Json::Value configs;
    try {
        thor::ResourceHolder<Json::Value, std::string> holder;
        configs = holder.acquire("config", Resources::loadJSON("Config.json"));
    }
    catch ( Json::RuntimeError e ) {
        std::cerr << "Something went wrong loading the Config.json:" << std::endl
            << e.what() << std::endl;
    }

    Application app(window, configs);

    sf::Clock clock;
    while ( window.isOpen() ) {
        window.clear();
        sf::Time elapsed = clock.restart();
        app.update(elapsed.asSeconds());
        window.display();
    }

    return 0;
}

// Copyright 2015-2016 Bablawn3d5

#include <boost/python.hpp>
#include <Farquaad/Serialization.hpp>
#include <json/json.h>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <entityx/entityx.h>
#include <Farquaad/Systems.hpp>
#include <Farquaad/Systems/PythonSystem.h>
#include <Farquaad/Components.hpp>
#include <Farquaad/Core.hpp>
#include <Farquaad/Thor/ResourceLoader.hpp>
#include <Farquaad/Box2D/SFMLB2DDebug.h>
#include <Thor/Resources.hpp>

#include <Farquaad/Systems/PythonSystem.h>
#include <vector>
#include <string>

namespace fs = boost::filesystem;
namespace py = boost::python;

// HACK(SMA) : Just shove this here for now.
BOOST_PYTHON_MODULE(_entityx_components) {
  Serializable::initPy(py::class_<Body>("Body", py::init<>()));
  Serializable::initPy(py::class_<Physics>("Physics", py::init<>()));
  Serializable::initPy(py::class_<Destroyed>("Destroyed", py::init<>()));
  Serializable::initPy(py::class_<Stats>("Stats", py::init<>()));
  Serializable::initPy(py::class_<InputResponder>("InputResponder", py::init<>()));
  Serializable::initPy(py::class_<Physics::CoollidingSet>("CollisionList", py::init<>()));
  typedef std::vector<std::string> vec_string;
  Serializable::initPy(py::class_<vec_string>("std_vector_string", py::init<>()));
  Serializable::initPy(py::enum_<b2BodyType>("b2BodyType"));
  Serializable::initPy(py::enum_<CollisionCategory>("CollisionCategory"));
  Serializable::initPy(py::class_<CollisionCategoryBitset>("CollisionCategoryBitset", py::init<>()));
  // This doesn't work
  //Serializable::initPy<sf::Vector2i>(
  //  py::class_<sf::Vector2i>("sf_vector_int", py::init<>()));

  // TODO(SMA) : Seralize me
  // TODO(SMA) : Add is_convertable between these two.
  py::class_< sf::Vector2<int> >("Vec2i", py::init<>())
    .def_readwrite("x", &sf::Vector2<int>::x)
    .def_readwrite("y", &sf::Vector2<int>::y);

  py::class_< sf::Vector2<float> >("Vec2f", py::init<>())
    .def_readwrite("x", &sf::Vector2<float>::x)
    .def_readwrite("y", &sf::Vector2<float>::y);
}

// Quick test for EntityX
class Application : public entityx::EntityX {
public:
    std::shared_ptr<b2World> physWorld;
    std::shared_ptr<SFMLB2DDebug> debugDraw;
    sf::RenderWindow& window;
    sf::View debugViewPort;

    explicit Application(fs::path exec_dir, sf::RenderWindow &target, Json::Value& v) : 
      window(target), debugViewPort(target.getView()) { // NOLINT
        // Setup Box2d Physics
        b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
        physWorld = std::make_shared<b2World>(gravity);
        auto physsystem = systems.add<PhysicsSystem>(physWorld);

        // Setup Debug drawing stuff
        const float ppm = physsystem->PIXELS_PER_METER;
        debugDraw = std::make_shared<SFMLB2DDebug>(target, ppm);
        physWorld->SetDebugDraw(debugDraw.get());
        debugDraw->SetFlags(b2Draw::e_shapeBit /*| b2Draw::e_aabbBit|
                                               b2Draw::e_centerOfMassBit | b2Draw::e_pairBit*/);

        // Setup Input
        auto inputSystem = systems.add<InputSystem>(target);
        inputSystem->setKeybinds(Serializable::fromJSON<InputSystem::KeyBindMap>(v["keys"]));

        // Setup the rest
        systems.add<SpriteRenderSystem>(target);
        systems.add<MoveSystem>();
        systems.add<DestroyerSystem>();
        systems.add<ImGuiSystem>(target);

        // Setup python system

        // Set python script path
        std::string path_exec = exec_dir.string();
        std::string path_scripts = (exec_dir / "scripts").string();
        assert(
          PyImport_AppendInittab("_entityx_components", init_entityx_components) != -1
          && "Failed to initialize _entityx_components Python module");
#ifdef NDEBUG
        PyImport_AppendInittab("_entityx_components", init_entityx_components);
#endif
        auto pythonSystem = systems.add<PythonSystem>(&entities, path_exec.c_str());
        pythonSystem->add_path(path_scripts.c_str());
        systems.configure();

        // Output script path
        std::cout << "Script path: [" << path_exec;
        for ( auto& path_str : pythonSystem->python_paths() ) {
          std::cout << "," << path_str;
        }
        std::cout << "]" << std::endl;

        // HACK(SMA) : Create entity right in this bloated constructor.
        thor::ResourceHolder<Json::Value, std::string> holder;
        for ( auto& items : v["entities"] ) {
            std::cout << "Loading file: " << items.asString() << std::endl;
            auto json = holder.acquire(items.asString(),
                                       Resources::loadJSON(items.asString()));
            EntitySerializer es(json);
            auto entity = entities.create();
            es.Load(entity);
        }
    }

    ~Application() {
    }

    void update(ex::TimeDelta dt) {
        systems.update<DestroyerSystem>(dt);
        systems.update<InputSystem>(dt);
        systems.update<MoveSystem>(dt);
        systems.update<PythonSystem>(dt);
        systems.update<PhysicsSystem>(dt);
        systems.update<SpriteRenderSystem>(dt);

        // Draw debug data literally ontop of every thing.
        physWorld->DrawDebugData();
        window.setView(window.getDefaultView());
        systems.update<ImGuiSystem>(dt);
    }
};

int main(int argc, char* const argv[]) {
    // global setup..
    auto execute_dir = fs::system_complete(argv[0]).remove_filename();
    sf::RenderWindow window(sf::VideoMode(800, 600), "");
    window.setKeyRepeatEnabled(true);

    // HACK(SMA) : Initalize these component serializers so they become registered.
    // FIXME(SMA) : These cause JSON objects to be held in memory permanantly. FIXME.
    {
        Serializable::handle<Body>();
        Serializable::handle<Destroyed>();
        Serializable::handle<Stats>();
        Serializable::handle<Physics>();
        Serializable::handle<PythonScript>();
        Serializable::handle<InputResponder>();
        Serializable::handle<Renderable>();
        Serializable::handle<sf::Color>();
        Serializable::handle<Physics::CoollidingSet>();
    }

    thor::ResourceHolder<Json::Value, std::string> holder;
    Json::Value configs = holder.acquire("config", Resources::loadJSON("Config.json"));
    const std::string title = configs["app_title"].asString();
    window.setTitle(title);
    Application app(execute_dir, window, configs);

    sf::Clock clock;
    const sf::Color clear_color = Serializable::fromJSON<sf::Color>(configs["clear_color"]);
    while ( window.isOpen() ) {
      window.clear(clear_color);
      sf::Time elapsed = clock.restart();
      app.update(elapsed.asSeconds());
      window.display();
    }

    return 0;
}

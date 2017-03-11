// Copyright 2017 Bablawn3d5

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>
#include <experimental/filesystem>
#include <Farquaad/Serialization.hpp>
#include <json/json.h>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <entityx/entityx.h>
#include <Farquaad/Systems.hpp>
#include <entityx/python/PythonSystem.h>
#include <Farquaad/Core.hpp>
#include <Farquaad/Thor/ResourceLoader.h>
#include <Farquaad/Core/LevelResourceLoader.h>
#include <Farquaad/Box2D/SFMLB2DDebug.h>
#include <Thor/Resources.hpp>

#include <vector>
#include <string>
#include <atomic>
#include <thread>

namespace fs = std::experimental::filesystem;
namespace py = pybind11;
using namespace std::chrono_literals;
using PythonSystem = entityx::python::PythonSystem;

// HACK(SMA) : Just shove this here for now.
namespace farqaad_python {

PYBIND11_PLUGIN(_entityx_components) {
  typedef std::vector<std::string> vec_string;
  py::module m("_entityx_components", "Module containing the running game's components");
  Serializable::initPy<Body>({ m, "Body" });
  Serializable::initPy<Sound>({m, "Sound"});
  Serializable::initPy<Physics>({m, "Physics"});
  Serializable::initPy<Destroyed>({m, "Destroyed"});
  Serializable::initPy<Stats>({m, "Stats"});
  Serializable::initPy<Renderable>({m, "Renderable"});
  Serializable::initPy<InputResponder>({m, "InputResponder"});
  Serializable::initPy<Physics::CoollidingSet>({m, "CollisionList"});
  Serializable::initPy<vec_string>({m, "std_vector_string"});
  Serializable::initPy<b2BodyType>({m, "b2BodyType"});
  Serializable::initPy<CollisionCategory>({m, "CollisionCategory"});
  Serializable::initPy<CollisionCategoryBitset>({m, "CollisionCategoryBitset"});
  // This doesn't work
  //Serializable::initPy<sf::Vector2i>(
  //  py::class_<sf::Vector2i>("sf_vector_int", py::init<>()));

  // TODO(SMA) : Seralize me
  // TODO(SMA) : Add is_convertable between these two.
  py::class_< sf::Vector2<int> >(m, "Vec2i")
    .def_readwrite("x", &sf::Vector2<int>::x)
    .def_readwrite("y", &sf::Vector2<int>::y);

  py::class_< sf::Vector2<float> >(m, "Vec2f")
    .def_readwrite("x", &sf::Vector2<float>::x)
    .def_readwrite("y", &sf::Vector2<float>::y);
  
  using FpSeconds =
    std::chrono::duration<float, std::chrono::seconds::period>;
  py::class_< entityx::TimeDelta >(m, "TimeDelta")
    .def("__init__",
      [](entityx::TimeDelta &dt) {
        FpSeconds(dt).count();
      });

  return m.ptr();
}

} //namespace farqaad_python

// Quick test for EntityX
class Application final : public entityx::EntityX {
public:
    sf::RenderWindow& window;
    // System ptrs.
    std::shared_ptr<b2World> physWorld;
    std::shared_ptr<SFMLB2DDebug> debugDraw;

    // General settings
    sf::View debugViewPort;
    bool debug = true;
    bool physDebug = true;
    ex::TimeDelta frameAdvance = std::chrono::seconds::zero();
    const sf::Color clear_color = sf::Color(50,120,170);


    // Resource loader stuff
    LevelResoruceLoader level;
    std::thread waitThread;
    std::thread loadThread;

    // Async locks and such for safety.
    // em_system_lock to represnet who can write to entity systems.
    std::mutex em_system_lock;
    std::atomic_bool isPaused{ false };
    std::atomic_bool isLoading{ true };
    std::atomic_bool showLoadscreen{ false };
    std::atomic_bool isRunning{ true };

    // HACK(SMA) : Magic loader entity.
    entityx::Entity magic_loader;

    auto queueResources(const Json::Value& json) {
      ComponentSerializer cs(json);
      // Do a bunch of bullshit to get stuff loaded.
      auto renderable = cs.Load<Renderable>();
      if ( renderable.texture_name.length() != 0 ) {
        level.queue_load(renderable.texture_name);
      }
      if ( renderable.font_name.length() != 0 ) {
        level.queue_load(renderable.font_name);
      }
      auto sound = cs.Load<Sound>();
      if ( sound.name.length() != 0 ) {
        level.queue_load(sound.name);
      }
      return cs;
    }

    explicit Application(const fs::path& exec_dir, sf::RenderWindow &target,const Json::Value& v, const sf::Color& clear) :
      window(target), debugViewPort(target.getView()), clear_color(clear){ // NOLINT

      //// HACK(SMA) : Initalize  python27.zip path here.

      // HACK(SMA) : Throw away wide characters
      const std::string path_exec = exec_dir.string();
      // Write to tempoaray char[] for PySys_SetPath
      std::vector<char> lib_paths(path_exec.begin(), path_exec.end());
      lib_paths.push_back('\0');
      // Set path so we can find the python27.zip
      Py_NoSiteFlag = 1;
      Py_SetPythonHome(&lib_paths[0]);
      Py_Initialize();

      // Assuming python27.zip is our python lib zip.
      PyRun_SimpleString("import sys");
      PyRun_SimpleString("sys.path.append('python27.zip')");

      try {
        farqaad_python::pybind11_init();
      }
      catch ( std::exception& e ) {
        std::cerr << e.what();
        throw e;
      }


      // Lock the system_lock so we have loadThread wait for 
      // systems to be fully loaded.
      std::atomic_flag system_lock = ATOMIC_FLAG_INIT;
      system_lock.test_and_set(std::memory_order_acquire);
      // Assume since we created this lock, we can be the ones to lock it without trying.

      std::cout << "Loading...:" << "\n";

      waitThread = std::thread([this] {
        while ( isRunning ) {
          level.loading(); 
        } 
      });
      waitThread.detach();

      loadThread = std::thread([this, &v, &system_lock] {
        // HACK(SMA) : Detect magic loader entity.
        for ( auto& item : v["loader"] ) {
          const auto& str = item.asString();
          level.queue_load(str);
        }

        // Wait till we're done loading initial list of entities.
        level.joins();

        // Wait till Application is finished configuring systems before we
        // load components in.
        while ( system_lock.test_and_set(std::memory_order_acquire) ) {};
        
        // Load magic loadscreen
        for ( auto& item : v["loader"] ) {
          auto json = level.get<Json::Value>(item.asString());
          // Load resources and consutct seralizer 
          EntitySerializer es(this->queueResources(json));
          // Lock systems as loading entites is not-threadsafe
          std::lock_guard<std::mutex>(this->em_system_lock);
          magic_loader = entities.create();
          es.Load(magic_loader);
        }

        // Wait till any residual stuff loads in
        level.joins();

        // Signal main thread we can have a loadscreen
        level.queue([this] {
          this->start_loadscreen();
        });

        // Load all entites in and preloader data
        for ( auto& item : v["entities"] ) {
          const auto& str = item.asString();
          level.queue_load(str);
        }

        // Wait till json files have loaded.
        level.joins();

        // Load Components
        for ( auto& item : v["entities"] ) {
          auto json = level.get<Json::Value>(item.asString());
          this->queueResources(json);
        }

        // Notify that loading is done
        level.queue([this, &v] {
          //// Lock systems as loading entites is not-threadsafe
          //std::lock_guard<std::mutex>(this->em_system_lock);
          //// Destory everyhting and load the game.
          //entities.reset();
          //for ( auto& item : v["entities"] ) {
          //  auto json = level.get<Json::Value>(item.asString());
          //  EntitySerializer es(json);
          //  auto ent = entities.create();
          //  es.Load(ent);
          //}
          // Signal game start.
          this->start_game();
        });
      });

      // Setup Box2d Physics
      b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
      physWorld = std::make_shared<b2World>(gravity);
      auto physsystem = systems.add<PhysicsSystem>(physWorld);

      // Setup Debug drawing stuff
      const float ppm = physsystem->PIXELS_PER_METER;
      debugDraw = std::make_shared<SFMLB2DDebug>(window, ppm);
      physWorld->SetDebugDraw(debugDraw.get());
      debugDraw->SetFlags(b2Draw::e_shapeBit /*| b2Draw::e_aabbBit|
                                              b2Draw::e_centerOfMassBit | b2Draw::e_pairBit*/);

      // Setup Input
      auto inputSystem = systems.add<InputSystem>(window);
      inputSystem->setKeybinds(Serializable::fromJSON<InputSystem::KeyBindMap>(v["keys"]));

      // Setup the rest
      systems.add<SoundSystem>(level);
      systems.add<SpriteRenderSystem>(window, level);
      systems.add<MoveSystem>();
      systems.add<DestroyerSystem>();
      systems.add<ImGuiSystem>(window);

      // Setup python system

      // Set python script path
      const std::string path_scripts = (exec_dir / "scripts").string();
      
      // Add python system
      auto pythonSystem = systems.add<PythonSystem>(entities);
      pythonSystem->add_path(path_scripts);
      systems.configure();

      // Output script path
      std::cout << "Script path: [" << path_exec;
      for ( auto& path_str : pythonSystem->python_paths() ) {
        std::cout << "," << path_str;
      }
      std::cout << "]" << std::endl;

      auto e = entities.create();
      e.assign<PythonScript>("system.loader", "Loader");
      
      // Release lock to let loading threads know systems are loaded.
      system_lock.clear();
    }

    ~Application() noexcept {
      // Signal threads to cleanup
      isRunning = false;
      if ( waitThread.joinable() ) { waitThread.join(); }
      if ( loadThread.joinable() ) { loadThread.join(); }
    }

    Application(const Application& rhs) = delete;             // prevent copying
    Application& operator=(const Application& rhs) = delete;  // prevent assignment

    void start_loadscreen() {
      this->showLoadscreen = true;
    }
    
    void start_game() {
      this->isLoading = false;
    }

    void update(ex::TimeDelta dt) {
        // Async loading state, locks evvreywhere.
        std::lock_guard<std::mutex>(this->em_system_lock);
        if( this->isLoading == true ){
            window.clear();
            if ( this->showLoadscreen == true ) {
              systems.update<InputSystem>(dt);
              // FIXME(SMA) : entityx::Python needs a bit of rework with threads
              // GIL needs to be unlocked on loading threads, and aquired here.
              // Might not be worth the fix if we change to pybind11.
              //systems.update<PythonSystem>(dt);
              systems.update<SpriteRenderSystem>(dt);
            }
            // Arificially slow down updates so we're not busy-waiting.
            window.display();
            return;
        }

        // Regular game state is lock free.
        window.clear(clear_color);

        // Don't process time if no time is passing.
        if( isPaused ) {
          dt = 0ms;
        }

        if ( frameAdvance.count() != 0.f ) {
          dt = frameAdvance;
          frameAdvance = frameAdvance.zero();
        }

        systems.update<InputSystem>(dt);
        systems.update<DestroyerSystem>(dt);
        systems.update<MoveSystem>(dt);
        systems.update<PythonSystem>(dt);
        systems.update<PhysicsSystem>(dt);
        systems.update<SpriteRenderSystem>(dt);
        systems.update<SoundSystem>(dt);

        // HACK(SMA) : Check if we need to toggle some hardcoded keys
        const auto& events = systems.system<InputSystem>()->triggedEvents;
        if ( std::find(events.begin(), events.end(), "+Game_Pause") != events.end() ) {
          isPaused = !isPaused;
        }

        if ( std::find(events.begin(), events.end(), "+Game_FrameAdvance") != events.end() ) {
          frameAdvance = 1270us;
        }

        if ( std::find(events.begin(), events.end(), "+Debug") != events.end() ) {
          debug = !debug;
        }
        if ( std::find(events.begin(), events.end(), "+Phys_Debug") != events.end() ) {
          physDebug = !physDebug;
        }

        // Draw debug data literally ontop of every thing.
        if ( this->debug ) {
          if ( this->physDebug ) {
            physWorld->DrawDebugData();
          }
          systems.update<ImGuiSystem>(dt);
          // Reset window state for next sprite window.draws.
          window.resetGLStates();
        }
        window.display();
    }
};

int main(int argc, char* const argv[]) {
    // global setup..
    const auto execute_dir = fs::system_complete(argv[0]).remove_filename();

    // Temparay JSON resoruce loader here just so we can load the conifg
    // to load our other resource loaders.
    thor::ResourceHolder<Json::Value, std::string> holder;
    Json::Value configs = holder.acquire("config", Resources::loadJSON("Config.json"));
    const std::string title = configs["app_title"].asString();
    const sf::Color clear_color = Serializable::fromJSON<sf::Color>(configs["clear_color"]);
    // To have app destory itself when window closes.
    sf::RenderWindow window(sf::VideoMode(1024, 768), "");
    window.setKeyRepeatEnabled(true);
    window.setTitle(title);
    // Scope to destory the app when main loop closes.
    {
      Application app(execute_dir, window, configs, clear_color);
      // FIXME(SMA) : Apparently high_resolution_clock isn't consistent on some platforms
      // but I have yet to observe that.
      auto start = std::chrono::steady_clock::now();
      static_assert(std::chrono::steady_clock::is_steady, "Clock should be steady");
      while ( window.isOpen() ) {
        auto finish = std::chrono::steady_clock::now();
        app.update(entityx::TimeDelta(finish - start));
        start = finish;
      }
    }
    return 0;
}

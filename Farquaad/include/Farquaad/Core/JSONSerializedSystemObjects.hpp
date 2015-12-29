// Copyright 2015 Bablawn3d5

#pragma once

#include <Farquaad/Core/SeralizeableComponentMap.h>
#include <Farquaad/Core/JSONSerializedPrimitiveTypes.hpp>
#include <Farquaad/Systems.hpp>
#include <Thor/Input.hpp>
#include <json/json.h>
#include <regex>
#include <string>

template<>
class SerializableHandle<KeyInput> {
public:
    // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
    // Define these so class becomes non POD for const initalizaiton
    SerializableHandle() {}
    ~SerializableHandle() {}

    inline KeyInput fromJSON(const Json::Value& v) const {
        KeyInput bind;
        auto keyString = v.asString();
        std::regex rek("^KEY_(.*)");
        std::regex rem("^MOUSE_(.*)");
        std::smatch match;
        if ( std::regex_search(keyString, match, rek) && match.size() > 1 ) {
            bind.userInputType = KeyboardInput;
            bind.keyCode = thor::toKeyboardKey(match[1].str());
        } else if ( std::regex_search(keyString, match, rem) && match.size() > 1 ) {
            bind.userInputType = MouseInput;
            bind.mouseButton = thor::toMouseButton(match[1].str());
        } else {
            // Do nothing
            (void)match;
        }
        return bind;
    }

    inline Json::Value toJSON(const KeyInput& bind) const {
        Json::Value v;
        if ( bind.userInputType == KeyboardInput ) {
            v = "KEY_" + thor::toString(bind.keyCode);
        } else if ( bind.userInputType == MouseInput ) {
            v = "MOUSE_" + thor::toString(bind.mouseButton);
        } else {
            // Never should happen.
            assert(!"Something has gone horribly wrong");
        }
        return v;
    }
};

template<>
class SerializableHandle<InputSystem::KeyBindMap> {
public:
    // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
    // Define these so class becomes non POD for const initalizaiton
    SerializableHandle() {}
    ~SerializableHandle() {}

    inline InputSystem::KeyBindMap fromJSON(const Json::Value& v) const {
        InputSystem::KeyBindMap map;
        for ( auto& val : v.getMemberNames() ) {
            KeyInput b = Serializable::fromJSON<KeyInput>(v[val]);

            // Handle a bit of the fragile string event system here.
            if ( b.userInputType == KeyboardInput ) {
                b.eventType = sf::Event::KeyPressed;
                map["+" + val] = b;

                b.eventType = sf::Event::KeyReleased;
                map["-" + val] = b;
            } else if ( b.userInputType == MouseInput ) {
                b.eventType = sf::Event::MouseButtonPressed;
                map["+" + val] = b;

                b.eventType = sf::Event::MouseButtonReleased;
                map["-" + val] = b;
            } else {
                // Do nothing
                (void)b;
            }
        }
        return map;
    }

    inline Json::Value toJSON(const InputSystem::KeyBindMap& map) const {
        Json::Value v;
        for ( auto& pair : map ) {
            // Handle a bit of the fragile string event system here.
            std::regex rek("^[+-](.*)");
            std::smatch match;
            if ( std::regex_search(pair.first, match, rek) && match.size() > 1 ) {
                v[match[1].str()] = Serializable::toJSON(pair.second);
            } else {
                // Never should happen.
                assert(!"Something has gone horribly wrong");
            }
        }
        return v;
    }
};

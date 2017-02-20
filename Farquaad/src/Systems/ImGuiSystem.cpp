// Copyright 2016 Bablawn3d5

#include <Farquaad/Serialization.hpp>
#include <Farquaad/Core/EntitySerializer.h>
#include <Farquaad/Systems/ImGuiSystem.h>
#include <json/json.h>
#include <SFML/System.hpp>

// Turn off a few warnings right here.. since we're doing a few things
// that may tigger warnings.
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifdef _MSC_VER
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#define snprintf _snprintf
#endif
#ifdef __clang__
#pragma clang diagnostic ignored "-Wold-style-cast"             // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wdeprecated-declarations"    // warning : 'xx' is deprecated: The POSIX name for this item.. // for strdup used in demo code (so user can copy & paste the code)
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"   // warning : cast to 'void *' from smaller integer type 'int'
#pragma clang diagnostic ignored "-Wformat-security"            // warning : warning: format string is not a string literal
#pragma clang diagnostic ignored "-Wexit-time-destructors"      // warning : declaration requires an exit-time destructor       // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. ImGui coding style welcomes static/globals.
#pragma clang diagnostic ignored "-Wreserved-id-macro"          // warning : macro name is a reserved identifier                //
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"          // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored "-Wformat-security"              // warning : format string is not a string literal (potentially insecure)
#pragma GCC diagnostic ignored "-Wdouble-promotion"             // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"                   // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#endif

void JsonToImGui(Json::Value v) {

}

void ImGuiSystem::update(ex::EntityManager & em,
                          ex::EventManager & events, ex::TimeDelta dt) {

//    ImGui::SFML::ProcessEvent(event);
    using FpSeconds =
      std::chrono::duration<float, std::chrono::seconds::period>;
    ImGui::SFML::Update( targ, sf::seconds(FpSeconds(dt).count()) );

    ImGui::SetNextWindowSize({ 200,120 }, ImGuiSetCond_FirstUseEver);
    ImGui::SetNextWindowPos({0,0}, ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Debug");
    ImGui::Text("FPS: %d", (int) (1.f/ FpSeconds(dt).count()) );

    auto all_entities = em.entities_for_debugging();
    size_t t = static_cast<size_t>(std::distance(all_entities.begin(), all_entities.end()));
    ImGui::Text("Entity Count: %zu", t );
    ImGui::NewLine();

    ImGui::SetNextWindowCollapsed(true, ImGuiSetCond_FirstUseEver);

    if ( ImGui::CollapsingHeader("Entites:") ) {
      EntitySerializer es;
      Json::StreamWriterBuilder builder;
      builder["precision"] = 9u;
      builder["commentStyle"] = "None";
      std::unique_ptr<Json::StreamWriter> writer(
        builder.newStreamWriter());
      std::stringstream stream;
      for ( auto entity : all_entities ) {
        stream << entity.id();
        if ( ImGui::TreeNode(stream.str().c_str()) ) {
          Json::Value object = es.Save(entity);
          stream.str(std::string());
          stream.clear();
          writer->write(object, &stream);
          ImGui::Text(stream.str().c_str());
          // Iterate though each component
          for ( auto& key : object.getMemberNames() ) {
            if ( ImGui::CollapsingHeader(key.c_str()) ) {
              //Itereate though each member
              // TODO(SMA) : Finish debug menu design, and fill this stuff out.
              // This does nothing for now.
              //Json::Value members = object[key];
              //for ( auto& member : members.getMemberNames() ) {
              //  // HACK(SMA) : Create one static box for all fields
              //  // we should really have one box per member
              //  static char buf[32] = "";
              //  ImGui::Text(member.c_str());
              //  ImGui::PushItemWidth(-1);
              //  ImGui::SameLine(); ImGui::InputText("## None", buf, 32);
              //  ImGui::PopItemWidth();
              //}
            }
          }
          ImGui::TreePop();
        }
        stream.str(std::string());
        stream.clear();
      }
    }
    ImGui::End();

    ImGui::Render();
}

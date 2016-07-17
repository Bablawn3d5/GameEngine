// Copyright 2015 Bablawn3d5

#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <cmath>

class SFMLB2DDebug : public b2Draw {
public:
    SFMLB2DDebug(sf::RenderTarget& target, const float scale) : target(target)
      , OUTLINE_THICKNESS(1.f/ scale) {}

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
    void DrawSolidCircle(const b2Vec2& center, float32 radius,
                         const b2Vec2& axis, const b2Color& color);
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    void DrawTransform(const b2Transform& xf);
private:
    sf::RenderTarget& target;
    const float OUTLINE_THICKNESS;
};

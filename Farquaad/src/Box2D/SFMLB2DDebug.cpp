// Copyright 2015 Bablawn3d5

#include <Farquaad/Box2D/SFMLB2DDebug.h>

sf::Color cast(const b2Color& color) {
    return sf::Color(static_cast<sf::Uint8>(color.r * 255),
                     static_cast<sf::Uint8>(color.g * 255),
                     static_cast<sf::Uint8>(color.b * 255),
                     static_cast<sf::Uint8>(color.a * 255));
}

sf::Vector2f cast(const b2Vec2& vector) {
    return sf::Vector2f(vector.x, vector.y);
}

void SFMLB2DDebug::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    sf::ConvexShape polygon(vertexCount);
    sf::Vector2f center;
    for ( int i = 0; i < vertexCount; i++ ) {
        sf::Vector2f& transformedVec = cast(vertices[i]);
        polygon.setPoint(i, transformedVec);
    }
    polygon.setOutlineThickness(-1.0f);
    polygon.setFillColor(sf::Color::Transparent);
    polygon.setOutlineColor(cast(color));

    target.draw(polygon);
}
void SFMLB2DDebug::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    sf::ConvexShape polygon(vertexCount);
    for ( int i = 0; i < vertexCount; i++ ) {
        sf::Vector2f& transformedVec = cast(vertices[i]);
        polygon.setPoint(i, transformedVec);
    }
    polygon.setOutlineThickness(-1.0f);
    sf::Color c = cast(color);
    polygon.setOutlineColor(c);
    c.a = 45;
    polygon.setFillColor(c);

    target.draw(polygon);
}
void SFMLB2DDebug::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
    sf::CircleShape circle(radius);
    circle.setOrigin(radius, radius);
    circle.setPosition(cast(center));
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(-1.0f);
    circle.setOutlineColor(cast(color));

    target.draw(circle);
}
void SFMLB2DDebug::DrawSolidCircle(const b2Vec2& center, float32 radius,
                                   const b2Vec2& axis, const b2Color& color) {
    sf::CircleShape circle(radius);
    circle.setOrigin(radius, radius);
    circle.setPosition(cast(center));
    sf::Color c = cast(color);
    c.a = 45;
    circle.setFillColor(c);
    circle.setOutlineThickness(1.0f);
    circle.setOutlineColor(cast(color));

    target.draw(circle);

    b2Vec2 endPoint = center + radius * axis;
    this->DrawSegment(center, endPoint, color);
}
void SFMLB2DDebug::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
    sf::Vertex line[] =
    {
        sf::Vertex(cast(p1), cast(color)),
        sf::Vertex(cast(p2), cast(color))
    };

    target.draw(line, 2, sf::Lines);
}
void SFMLB2DDebug::DrawTransform(const b2Transform& xf) {
    const float lineLength = 10.0f;

    b2Vec2 xAxis = xf.p + lineLength * xf.q.GetXAxis();
    b2Vec2 yAxis = xf.p + lineLength * xf.q.GetYAxis();
    b2Color red(1.0f, 0.0f, 0.0f);
    b2Color green(0.0f, 1.0f, 0.0f);

    this->DrawSegment(xf.p, xAxis, red);
    this->DrawSegment(xf.p, yAxis, green);
}

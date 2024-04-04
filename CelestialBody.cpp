// Copyright 2024 Samuel Stanley

#include <iostream>
#include <string>
#include <memory>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "Constants.hpp"
#include "CelestialBody.hpp"
#include "Universe.hpp"

namespace NB {

std::istream& operator>>(std::istream& in, CelestialBody& celestialbody) {
    // Get position
    in >> celestialbody._position.x >> celestialbody._position.y;

    // Get velocity
    in >> celestialbody._velocity.x >> celestialbody._velocity.y;

    // Get mass
    in >> celestialbody._mass;

    // Get texture
    std::string file_name;
    in >> file_name;
    celestialbody._texture = celestialbody._universe.getTexture(file_name);
    celestialbody._sprite = std::make_unique<sf::Sprite>();
    celestialbody._sprite->setTexture(*(celestialbody._texture));
    sf::Vector2u texture_size = celestialbody._texture->getSize();
    celestialbody._sprite->setOrigin(texture_size.x / 2.0, texture_size.y / 2.0);

    return in;
}

std::ostream& operator<<(std::ostream& out, const CelestialBody& celestialbody) {
    // Output position
    out << std::scientific << celestialbody.position().x << ' ';
    out << std::scientific << celestialbody.position().y << ' ';

    // Output velocity
    out << std::scientific << celestialbody.velocity().x << ' ';
    out << std::scientific << celestialbody.velocity().y << ' ';

    // Output mass
    out << std::scientific << celestialbody.mass() << ' ';

    // Output texture name
    out << celestialbody._universe.getTextureName(celestialbody._texture);

    return out;
}

void CelestialBody::step(double seconds) {
    // Get component forces
    sf::Vector2<double> force = _universe.getForce(*this);

    // Calculate new velocity
    _velocity.x += seconds * (force.x / _mass);
    _velocity.y += seconds * (force.y / _mass);

    // Calculate new position
    _position.x += seconds * _velocity.x;
    _position.y += seconds * _velocity.y;
}

sf::Vector2<double> CelestialBody::distance(const CelestialBody& body1,
    const CelestialBody& body2) {
    sf::Vector2<double> distance;
    distance.x = body2._position.x - body1._position.x;
    distance.y = body2._position.y - body1._position.y;
    return distance;
}

bool operator==(const CelestialBody& body1, const CelestialBody& body2) {
    return body1._position == body2._position
        && body1._velocity == body2._velocity
        && body1._mass == body2._mass;
}

void CelestialBody::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Calculate sprite position
    sf::Vector2<double> sprite_pos;
    sprite_pos.x = (_position.x / _universe.radius()) * (target.getSize().x / 2.0);
    sprite_pos.x += target.getSize().x / 2.0;
    sprite_pos.y = (_position.y / _universe.radius()) * -(target.getSize().y / 2.0);
    sprite_pos.y += target.getSize().y / 2.0;
    _sprite->setPosition(static_cast<sf::Vector2f>(sprite_pos));

    // Draw sprite
    target.draw(*_sprite, states);
}

}  // namespace NB

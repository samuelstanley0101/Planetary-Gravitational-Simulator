// Copyright 2024 Samuel Stanley

#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include "ForwardDeclarations.hpp"

namespace NB {

class CelestialBody : public sf::Drawable {
 public:
    // Construct a CelestialBody with default values
    explicit CelestialBody(Universe& universe) : _universe(universe) {}

    // Give celestialbody the values read from in
    friend std::istream& operator>>(std::istream& in, CelestialBody& celestialbody);

    // Write the current state of universe to out
    friend std::ostream& operator<<(std::ostream& out, const CelestialBody& celestialbody);

    // Update the position and velocity according to the force calculated by the universe and the
    // amount of seconds given since the CelestialBody was last updated
    void step(double seconds);

    // Return the current position of the CelestialBody
    sf::Vector2f position() const { return static_cast<sf::Vector2f>(_position); }

    // Return the current velocity of the CelestialBody
    sf::Vector2f velocity() const { return static_cast<sf::Vector2f>(_velocity); }

    // Return the mass of the CelestialBody
    double mass() const { return _mass; }

    // Return the distance between body2 and body1 (body2 - body1)
    static sf::Vector2<double> distance(const CelestialBody& body1, const CelestialBody& body2);

    // Return if the positions, velocities, and masses are the same
    friend bool operator==(const CelestialBody& body1, const CelestialBody& body2);

 protected:
    // Draw the CelestialBody to the target with viewport position calculated from CelestialBody
    // position and Universe radius
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 private:
    sf::Vector2<double> _position;
    sf::Vector2<double> _velocity;
    double _mass;
    Universe& _universe;
    std::shared_ptr<sf::Texture> _texture;
    std::unique_ptr<sf::Sprite> _sprite;
};

}  // namespace NB

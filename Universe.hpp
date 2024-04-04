// Copyright 2024 Samuel Stanley

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <utility>
#include <SFML/Graphics.hpp>
#include "CelestialBody.hpp"

namespace NB {

class Universe : public sf::Drawable {
 public:
    // Construct a Universe object with default values
    Universe() : _radius(0.0), _calculatedForces(false) {}

    // Construct a Universe object with initial values from file_name
    explicit Universe(const std::string& file_name);

    // Give universe the values read from in
    friend std::istream& operator>>(std::istream& in, Universe& universe);

    // Write the current state of universe to out
    friend std::ostream& operator<<(std::ostream& out, const Universe& universe);

    // Update the positions and velocities of every particle in the universe given the amount of
    // seconds since the universe was last updated
    void step(double seconds);

    // Calculate the forces between every particle in the universe and store them
    void calculate_forces();

    // Return the number of particles in the Universe
    unsigned int numPlanets() const { return _forces.size(); }

    // Return the radius of the Universe
    double radius() const { return _radius; }

    // Return the particle at index n
    CelestialBody& operator[](size_t n) const { return *(_forces[n].first); }

    // Get the texture from file_name and add it to the map if it's not already in the map
    std::shared_ptr<sf::Texture> getTexture(const std::string& file_name);

    // Get the name of the texture given if its in the map. If not, std::out_of_range is thrown
    std::string getTextureName(std::shared_ptr<sf::Texture> texture) const;

    // Get the force for the CelestialBody. If the stored forces are out of date, calculate_forces
    // is called
    sf::Vector2<double> getForce(const CelestialBody& body);

    // Return if _forces is up to date
    bool calculatedForces() const { return _calculatedForces; }

 protected:
    // Draw every object in the Universe to the target
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 private:
    double _radius;
    std::unique_ptr<sf::Sprite> _background;
    std::map<std::string, std::shared_ptr<sf::Texture>> _textures;
    std::vector<std::pair<std::shared_ptr<CelestialBody>, sf::Vector2<double>>> _forces;
    bool _calculatedForces;
};

}  // namespace NB

// Copyright 2024 Samuel Stanley

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Universe.hpp"
#include "CelestialBody.hpp"
#include "Constants.hpp"

namespace NB {

Universe::Universe(const std::string& file_name) {
    std::ifstream fin(file_name);
    fin >> *this;
}

std::istream& operator>>(std::istream& in, Universe& universe) {
    // Set background texture
    universe._background = std::make_unique<sf::Sprite>();
    universe._background->setTexture(*universe.getTexture(background));
    sf::Vector2u texture_size = universe.getTexture(background)->getSize();
    universe._background->setOrigin(texture_size.x / 2.0, texture_size.y / 2.0);

    // Get input data
    size_t num_particles;
    in >> num_particles >> universe._radius;
    for (size_t i = 0; i < num_particles; i++) {
        universe._forces.push_back(std::pair<std::shared_ptr<CelestialBody>, sf::Vector2<double>>(
            std::make_shared<CelestialBody>(universe), sf::Vector2<double>(0, 0)));
        in >> universe[universe.numPlanets() - 1];
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const Universe& universe) {
    out << universe.numPlanets() << std::endl;
    if (universe.radius() > 1000)
        out << std::scientific;
    out << universe.radius() << std::endl;
    for (size_t i = 0; i < universe.numPlanets(); i++) {
        out << universe[i] << std::endl;
    }
    return out;
}

void Universe::step(double seconds) {
    _calculatedForces = false;

    for (size_t i = 0; i < numPlanets(); i++) {
        (*this)[i].step(seconds);
    }
}

void Universe::calculate_forces() {
    sf::Vector2<double> distance;
    double distance_sqrd;
    double total_distance;
    double total_force;

    // Set all stored forces to {0, 0}
    for (auto& force : _forces) {
        force.second = {0, 0};
    }

    // Calculate all forces
    for (size_t i = 0; i < numPlanets(); i++) {
        for (size_t j = i + 1; j < numPlanets(); j++) {
            // Calculate distances between Universe[i] and Universe[j]
            distance = CelestialBody::distance((*this)[i], (*this)[j]);
            distance_sqrd = (distance.x * distance.x) + (distance.y * distance.y);
            total_distance = sqrt(distance_sqrd);

            // Calculate force
            total_force = G * ((*this)[i].mass() * (*this)[j].mass()) / distance_sqrd;

            // Calculate x and y forces
            _forces[i].second.x += total_force * (distance.x / total_distance);
            _forces[i].second.y += total_force * (distance.y / total_distance);

            _forces[j].second.x -= total_force * (distance.x / total_distance);
            _forces[j].second.y -= total_force * (distance.y / total_distance);
        }
    }

    _calculatedForces = true;
}

std::shared_ptr<sf::Texture> Universe::getTexture(const std::string& file_name) {
    // if the texture from file_name is not in the map
    if (!_textures.count(file_name)) {
        _textures[file_name] = std::make_shared<sf::Texture>();
        // Check sub directory for file first and then base directory
        if (!_textures[file_name]->loadFromFile(image_dir + file_name) &&
            !_textures[file_name]->loadFromFile(file_name)) {
            // throw std::invalid_argument("Error: file '" + file_name + "' could not be found");
            std::cout << "Error: file '" << file_name << "' could not be found" << std::endl;
        }
    }
    return _textures[file_name];
}

std::string Universe::getTextureName(std::shared_ptr<sf::Texture> texture) const {
    auto map_item = std::find_if(_textures.begin(), _textures.end(), [texture](auto t) {
        return t.second == texture;
    });
    if (map_item == _textures.end())
        throw std::out_of_range("Error: texture name not found");
    return map_item->first;
}

sf::Vector2<double> Universe::getForce(const CelestialBody& body) {
    if (!_calculatedForces)
        calculate_forces();

    auto item = std::find_if(_forces.begin(), _forces.end(), [&body](auto i) {
        return *(i.first) == body;
    });
    return item->second;
}

void Universe::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Draw background
    _background->setPosition(target.getSize().x / 2.0, target.getSize().y / 2.0);
    target.draw(*_background);

    // Draw every particle
    for (size_t i = 0; i < numPlanets(); i++) {
        target.draw((*this)[i], states);
    }
}

}  // namespace NB

// Copyright 2024 Samuel Stanley

#include <iostream>
#include <stdexcept>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Universe.hpp"
#include "CelestialBody.hpp"
#include "Constants.hpp"

int main(int argc, char* argv[]) {
    // Get command line arguments
    if (argc < 3) {
        throw std::invalid_argument("Error: insufficient command line arguments");
    }
    double T = std::stod(std::string(argv[1]));
    double dt = std::stod(std::string(argv[2]));

    NB::Universe universe;
    std::cin >> universe;

    sf::VideoMode mode(800, 800);
    sf::RenderWindow window(mode, "NBody Simulation");

    double time_passed = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (time_passed >= T)
            window.close();

        universe.step(dt);

        window.clear();
        window.draw(universe);
        window.display();

        time_passed += dt;
    }

    std::cout << universe;

    return 0;
}

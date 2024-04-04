// Copyright 2024 Samuel Stanley

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <limits>
#include "Universe.hpp"
#include "CelestialBody.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(noParticles) {
    NB::Universe universe("tests/0body.txt");
    BOOST_CHECK_EQUAL(universe.numPlanets(), 0);
    BOOST_CHECK_CLOSE(universe.radius(), 100.0, 0.001);
}

BOOST_AUTO_TEST_CASE(oneParticle) {
    NB::Universe universe("tests/1body.txt");
    BOOST_CHECK_EQUAL(universe.numPlanets(), 1);
    BOOST_CHECK_CLOSE(universe.radius(), 100.0, 0.001);

    BOOST_CHECK_CLOSE(universe[0].position().x, 10.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].position().y, 20.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().x, 2.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().y, 1.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].mass(), 1e20, 0.001);
}

BOOST_AUTO_TEST_CASE(oneParticleFromStream) {
    std::ifstream fin("tests/1body.txt");
    NB::Universe universe;
    fin >> universe;
    BOOST_CHECK_EQUAL(universe.numPlanets(), 1);
    BOOST_CHECK_CLOSE(universe.radius(), 100.0, 0.001);

    BOOST_CHECK_CLOSE(universe[0].position().x, 10.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].position().y, 20.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().x, 2.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().y, 1.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].mass(), 1e20, 0.001);
}

BOOST_AUTO_TEST_CASE(threeParticles) {
    NB::Universe universe("tests/3body.txt");
    BOOST_CHECK_EQUAL(universe.numPlanets(), 3);
    BOOST_CHECK_CLOSE(universe.radius(), 1.25e11, 0.001);

    BOOST_CHECK_CLOSE(universe[0].position().x, 0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].position().y, 0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().x, 500, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().y, 0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].mass(), 5.974e24, 0.001);

    BOOST_CHECK_CLOSE(universe[1].position().x, 0, 0.001);
    BOOST_CHECK_CLOSE(universe[1].position().y, 4.5e10, 0.001);
    BOOST_CHECK_CLOSE(universe[1].velocity().x, 3e4, 0.001);
    BOOST_CHECK_CLOSE(universe[1].velocity().y, 0, 0.001);
    BOOST_CHECK_CLOSE(universe[1].mass(), 1.989e30, 0.001);

    BOOST_CHECK_CLOSE(universe[2].position().x, 0, 0.001);
    BOOST_CHECK_CLOSE(universe[2].position().y, -4.5e10, 0.001);
    BOOST_CHECK_CLOSE(universe[2].velocity().x, -3e4, 0.001);
    BOOST_CHECK_CLOSE(universe[2].velocity().y, 0, 0.001);
    BOOST_CHECK_CLOSE(universe[2].mass(), 1.989e30, 0.001);
}

BOOST_AUTO_TEST_CASE(insertionTest) {
    std::ifstream fin("tests/1body.txt");
    NB::Universe universe;
    fin >> universe;
    BOOST_REQUIRE_EQUAL(universe.numPlanets(), 1);
    BOOST_REQUIRE_CLOSE(universe.radius(), 100.0, 0.001);

    BOOST_REQUIRE_CLOSE(universe[0].position().x, 10.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].position().y, 20.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].velocity().x, 2.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].velocity().y, 1.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].mass(), 1e20, 0.001);

    std::stringstream sstream;
    sstream << universe;
    unsigned int num_planets;
    sstream >> num_planets;
    BOOST_CHECK_EQUAL(universe.numPlanets(), num_planets);
    double radius;
    sstream >> radius;
    BOOST_CHECK_CLOSE(universe.radius(), radius, 0.001);
    sf::Vector2f position;
    sstream >> position.x >> position.y;
    BOOST_CHECK_CLOSE(universe[0].position().x, position.x, 0.001);
    BOOST_CHECK_CLOSE(universe[0].position().y, position.y, 0.001);
    sf::Vector2f velocity;
    sstream >> velocity.x >> velocity.y;
    BOOST_CHECK_CLOSE(universe[0].velocity().x, velocity.x, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().y, velocity.y, 0.001);
    double mass;
    sstream >> mass;
    BOOST_CHECK_CLOSE(universe[0].mass(), mass, 0.001);
    std::string file_name;
    sstream >> file_name;
    BOOST_CHECK_EQUAL("earth.gif", file_name);
}

BOOST_AUTO_TEST_CASE(motionless) {
    std::ifstream fin("tests/motionless.txt");
    NB::Universe universe;
    fin >> universe;
    BOOST_REQUIRE_EQUAL(universe.numPlanets(), 1);
    BOOST_REQUIRE_CLOSE(universe.radius(), 1.0e11, 0.001);

    BOOST_REQUIRE_CLOSE(universe[0].position().x, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].position().y, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].velocity().x, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].velocity().y, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].mass(), 1e10, 0.001);

    universe.step(1000000000);
    BOOST_CHECK_CLOSE(universe[0].position().x, 0.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].position().y, 0.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().x, 0.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().y, 0.0, 0.001);
}

BOOST_AUTO_TEST_CASE(simpleMotion) {
    std::ifstream fin("tests/simpleMotion.txt");
    NB::Universe universe;
    fin >> universe;
    BOOST_REQUIRE_EQUAL(universe.numPlanets(), 1);
    BOOST_REQUIRE_CLOSE(universe.radius(), 1.0e6, 0.001);

    BOOST_REQUIRE_CLOSE(universe[0].position().x, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].position().y, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].velocity().x, 100.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].velocity().y, 100.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].mass(), 1e10, 0.001);

    universe.step(1);
    BOOST_CHECK_CLOSE(universe[0].position().x, 100.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].position().y, 100.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().x, 100.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().y, 100.0, 0.001);

    universe.step(999);
    BOOST_CHECK_CLOSE(universe[0].position().x, 100000.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].position().y, 100000.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().x, 100.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().y, 100.0, 0.001);
}

BOOST_AUTO_TEST_CASE(horizontalGravity) {
    std::ifstream fin("tests/horizontalGravity.txt");
    NB::Universe universe;
    fin >> universe;
    BOOST_REQUIRE_EQUAL(universe.numPlanets(), 2);
    BOOST_REQUIRE_CLOSE(universe.radius(), 200, 0.001);

    BOOST_REQUIRE_CLOSE(universe[0].position().x, -100, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].position().y, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].velocity().x, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].velocity().y, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].mass(), 1.0e4, 0.001);

    BOOST_REQUIRE_CLOSE(universe[1].position().x, 100, 0.001);
    BOOST_REQUIRE_CLOSE(universe[1].position().y, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[1].velocity().x, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[1].velocity().y, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[1].mass(), 5.0e4, 0.001);

    universe.step(100000);
    BOOST_CHECK_CLOSE(universe[0].position().x, -99.16625, 0.001);
    BOOST_CHECK_CLOSE(universe[0].position().y, 0.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().x, 8.3375e-6, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().y, 0.0, 0.001);

    BOOST_CHECK_CLOSE(universe[1].position().x, 99.83325, 0.001);
    BOOST_CHECK_CLOSE(universe[1].position().y, 0.0, 0.001);
    BOOST_CHECK_CLOSE(universe[1].velocity().x, -1.6675e-6, 0.001);
    BOOST_CHECK_CLOSE(universe[1].velocity().y, 0.0, 0.001);
}

BOOST_AUTO_TEST_CASE(verticalGravity) {
    std::ifstream fin("tests/verticalGravity.txt");
    NB::Universe universe;
    fin >> universe;
    BOOST_REQUIRE_EQUAL(universe.numPlanets(), 2);
    BOOST_REQUIRE_CLOSE(universe.radius(), 200, 0.001);

    BOOST_REQUIRE_CLOSE(universe[0].position().x, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].position().y, -100, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].velocity().x, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].velocity().y, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].mass(), 1.0e4, 0.001);

    BOOST_REQUIRE_CLOSE(universe[1].position().x, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[1].position().y, 100, 0.001);
    BOOST_REQUIRE_CLOSE(universe[1].velocity().x, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[1].velocity().y, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[1].mass(), 5.0e4, 0.001);

    universe.step(100000);
    BOOST_CHECK_CLOSE(universe[0].position().x, 0.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].position().y, -99.16625, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().x, 0.0, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().y, 8.3375e-6, 0.001);

    BOOST_CHECK_CLOSE(universe[1].position().x, 0.0, 0.001);
    BOOST_CHECK_CLOSE(universe[1].position().y, 99.83325, 0.001);
    BOOST_CHECK_CLOSE(universe[1].velocity().x, 0.0, 0.001);
    BOOST_CHECK_CLOSE(universe[1].velocity().y, -1.6675e-6, 0.001);
}

BOOST_AUTO_TEST_CASE(parallelVelocity) {
    std::ifstream fin("tests/parallelVelocity.txt");
    NB::Universe universe;
    fin >> universe;
    BOOST_REQUIRE_EQUAL(universe.numPlanets(), 3);
    BOOST_REQUIRE_CLOSE(universe.radius(), 2.0e10, 0.001);

    BOOST_REQUIRE_CLOSE(universe[0].position().x, -1.0e10, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].position().y, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].velocity().x, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].velocity().y, -1.0e5, 0.001);
    BOOST_REQUIRE_CLOSE(universe[0].mass(), 1.0e20, 0.001);

    BOOST_REQUIRE_CLOSE(universe[1].position().x, 1.0e10, 0.001);
    BOOST_REQUIRE_CLOSE(universe[1].position().y, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[1].velocity().x, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[1].velocity().y, 1.0e5, 0.001);
    BOOST_REQUIRE_CLOSE(universe[1].mass(), 1.0e20, 0.001);

    BOOST_REQUIRE_CLOSE(universe[2].position().x, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[2].position().y, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[2].velocity().x, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[2].velocity().y, 0.0, 0.001);
    BOOST_REQUIRE_CLOSE(universe[2].mass(), 1.0e28, 0.001);

    universe.step(100000);
    BOOST_CHECK_CLOSE(universe[0].position().x, -9.9333e9, 0.001);
    BOOST_CHECK_CLOSE(universe[0].position().y, -1e10, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().x, 6.670000016675e2, 0.001);
    BOOST_CHECK_CLOSE(universe[0].velocity().y, -1e5, 0.001);

    BOOST_CHECK_CLOSE(universe[1].position().x, 9.9333e9, 0.001);
    BOOST_CHECK_CLOSE(universe[1].position().y, 1e10, 0.001);
    BOOST_CHECK_CLOSE(universe[1].velocity().x, -6.670000016675e2, 0.001);
    BOOST_CHECK_CLOSE(universe[1].velocity().y, 1e5, 0.001);

    BOOST_CHECK_CLOSE(universe[2].position().x, 0.0, 0.001);
    BOOST_CHECK_CLOSE(universe[2].position().y, 0.0, 0.001);
    BOOST_CHECK_CLOSE(universe[2].velocity().x, 0.0, 0.001);
    BOOST_CHECK_CLOSE(universe[2].velocity().y, 0.0, 0.001);
}

# Planetary-Gravitational-Simulator
Created March of 2024

A C++ N-Body simulation to approximate and render using Simple Fast Media Library (SFML) the motion of an unlimited number of particles and the forces between them.

This program takes input from stdin in the format described below and approximates a simulation of the universe given an amount of time between simulation steps and a total duration of the simulation from the command line. Images for the planets' textures should be in a directory called "nbody", although the name of this directory can be changed in the image_dir variable in Constants.hpp, and the program will display an error message but still work if the images are in the base directory. Test files should be found in a directory called "Test Files". After the program has finished running, the final state of the universe will be output to stdout in the same format it was input.

Input file format:
integer N number of particles
real number radius of the universe
N rows:
x initial position    y initial position    x initial velocity     y initial velocity    mass    filename for image texture

Syntax: ./NBody (total simulation duration in seconds) (time in seconds between simulation steps) < (filename of input file)

This program requires the use of Simple Fast Media Library (SFML), which can be downloaded here: https://www.sfml-dev.org/download/sfml/2.6.1/
This program requires the use of the Boost testing library, which can be downloaded here: https://boostorg.jfrog.io/artifactory/main/release/1.84.0/source/

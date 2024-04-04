CC = g++
CFLAGS = --std=c++17 -Wall -Werror -pedantic -g
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lboost_unit_test_framework
# Your .hpp files
DEPS = Universe.hpp CelestialBody.hpp ForwardDeclarations.hpp Constants.hpp
# Your compiled .o files
OBJECTS = Universe.o CelestialBody.o
# The name of your program
PROGRAM = NBody
TEST = test
# The name of the library to be compiled
MYLIB = NBody.a

.PHONY: all clean lint


all: $(PROGRAM) $(TEST) $(MYLIB)

# Wildcard recipe to make .o files from corresponding .cpp file
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

$(TEST): test.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

$(MYLIB): $(OBJECTS)
	ar rcs $@ $(OBJECTS)

clean:
	rm *.o $(PROGRAM) $(TEST) $(MYLIB)

lint:
	cpplint *.cpp *.hpp

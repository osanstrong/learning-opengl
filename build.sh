#!/bin/bash

# Last commands used to compile, first to so file
# g++ -O0 -g3 -o build/lib/GL01Hello.so src/GL01Hello.cpp -Iinclude/ -lglut -lGL
# And then to final executable (note to self find a font that distinguished I and l)
# g++ -o build/main build/lib/GL01Hello.so

# Todo later: Actually figure out how to split this into so and executable commandss
g++ -O0 -g3 -o build/main src/GL01Hello.cpp -lglut -lGL -Iinclude/
build/main
all:jet

jet: main.cc *.h
	clang++ main.cc -o jet --std=c++1z -Wall -Wextra -lglfw -lGL -lGLEW -lm

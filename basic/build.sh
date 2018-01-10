#!/bin/bash
g++ -c main.cpp
g++ main.o -lGL -lSDL2 -lGLEW

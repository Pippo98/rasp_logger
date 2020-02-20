#!/bin/bash

gcc $1 -o deployable -lwiringPi -lm -lpthread -lstdc++ -lLSM9DS0.h
echo "COMPILED"
echo "Start Program"
./deployable
echo "Program Ended"

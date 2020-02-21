#!/bin/bash

gcc $1 -o deployable -lwiringPi -lm -lpthread -lstdc++
echo "COMPILED"
echo "Start Program"
./deployable
echo "Program Ended"

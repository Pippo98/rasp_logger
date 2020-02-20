#!/bin/bash

gcc $1 -o deployable -lwiringPi -lm -lpthread -lstdc++

ar rcs libout.a out.o

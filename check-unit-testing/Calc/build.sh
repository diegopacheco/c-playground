#!/bin/bash

gcc src/calc.h src/calc.c tests/testCalc.c -lcheck -lsubunit -lm -lrt -lpthread -o unit_tests
chmod +x unit_tests
./unit_tests
rm -rf unit_tests

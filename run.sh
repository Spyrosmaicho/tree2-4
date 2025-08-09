#!/bin/bash

make

valgrind --leak-check=full ./main < tests/input.txt
 
make clean
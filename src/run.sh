#!/bin/bash

make

valgrind --leak-check=full ./main < input.txt
 
make clean
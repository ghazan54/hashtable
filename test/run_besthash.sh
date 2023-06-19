#!/bin/bash

gcc -Wall besthash.c -I ../src/ ../src/*.c -o besthash.out
./besthash.out
python3 time.py
python3 col.py
mv time.png ../data/
mv col.png ../data/

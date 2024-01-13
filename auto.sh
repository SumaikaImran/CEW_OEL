#!/bin/bash
cd /home/sumaika/Downloads
gcc -o program oel.c -lcurl -ljson-c
./program
exit 0

#!/bin/bash

# main
rm -f ./pHAL.c
rm -f ./pHAL.h
cp ./../pHAL/pHAL.c ./
cp ./../pHAL/pHAL.h ./

make $@


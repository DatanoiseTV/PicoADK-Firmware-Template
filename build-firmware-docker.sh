#!/bin/sh
# Script to build the PicoADK firmware using the Docker Image
docker run --rm -u $(id -u):$(id -g)  -v $PWD:/project -w /project datanoisetv/picoadk-dev:latest build-firmware.sh

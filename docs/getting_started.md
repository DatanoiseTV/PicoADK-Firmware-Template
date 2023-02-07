# Getting started

This quick start guide will show you how to get started with your PicoADK board.
The guide should work on Windows, MacOS and Linux.

## Prerequisites

1. Install Docker - for beginners we suggest using [Docker Desktop](https://www.docker.com/products/docker-desktop/)

## Repository structure
* C / C++ code is located in the **src** folder and contains all the low-level funtionality for the firmware incl. USB MIDI handling and dealing with the Audio DAC.
* Includes for the C / C++ code are located in the **include** folder.
* The Vult DSP code is located in the **vultsrc** folder.
* The includes for Vult are located in **lib/vult/examples**. These provide some oscillator, filter and envelope implementation along others.

## Compiling

### Using the script

Just execute
```bash
./build-firmware-docker.sh
```
in the project folder.

### Manually
1. git clone --recursive https://github.com/DatanoiseTV/PicoADK-Firmware-Template.git
2. cd PicoADK-Firmware-Template
3. Enter the following command in your project directory:

```bash
docker run --rm -u $(id -u):$(id -g)  -v $PWD:/project -w /project datanoisetv/picoadk-dev:latest build-firmware.sh
```

## Uploading

After your compile process was successful, you will see a *.UF2 file in the build subdirectory.
Press and hold the **BOOT** button on your PicoADK Hardware and press the **RESET** button while holding **BOOT**.
Then release **BOOT** and a virtual drive will appear.

Now you can simply drag and drop the *.UF2 file to the virtual disk drive and the PicoADK will reboot
and start running your firmware.
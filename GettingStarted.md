# Introduction

The PicoADK-FreeRTOS template is one of the (more advanced) ways to get started with the PicoADK. It is based around the Raspberry Pico SDK and CMake.

## Prerequisites

1. Install the  vult compiler: `sudo npm install vult -g`

## Building 
```
git submodule update --init --recursive
mkdir build && cd build
cmake ..
make
cp rp2040-vult.uf2 /media/xyz/RP2_BOOT/
```

### Caveats in the code

Some steps how to be performed for the peripherals of the board to function properly.

* If you want to use the 4 LEDs next to the USB Type-C connector, you must set GPIO2-5 (shared) as output.
* If you want to use the Audio DAC, your software needs to control the XSMT signal (see [PicoADK Hardware Repo](https://github.com/DatanoiseTV/PicoADK-Hardware))
* If you are running the CPU cores at more than 133 MHz, it might be necessary to lower the Flash Clock Speed. This is done [here](https://github.com/DatanoiseTV/PicoADK-FreeRTOS-Template/blob/main/CMakeLists.txt#L118).
* 

### Structure

The main code, which does hardware initialization and launches the FreeRTOS tasks for dealing with USB MIDI etc. is located in src/main.cpp.
You can find the callback used to call the DSP process function [here](https://github.com/DatanoiseTV/PicoADK-FreeRTOS-Template/blob/main/src/main.cpp#L198).
By default, this is calling the process function of [Vult DSP](https://www.vult-dsp.com/vult-language) code. If you want to look into the code, check out
the "vultsrc" folder.


### Vult Language references
* [Vult Language Basics](https://vult-dsp.github.io/vult/tutorials/basics/)
* [Easy DSP with Vult](https://vult-dsp.github.io/vult/tutorials/dsp/)

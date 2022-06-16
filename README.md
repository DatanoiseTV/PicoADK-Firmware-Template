# Raspberry Pi RP2040 MCU Vult DSP template with FreeRTOS

![PCB](https://github.com/DatanoiseTV/RP2040-DSP-FreeRTOS-Template/raw/main/hardware/image.png)

This C++ boilerplate template allows for creating Synthesizers, Effects and other Audio DSP related projects based on the cheap and easily available RP2040 MCU. It targets the "RP2040 Eurorack DSP Demo Board", which I have made.

The DSP board contains:

* RP2040 Dual-Core MCU (running at 400MHz)
* MIDI input (5-pin DIN)
* Stereo 3,5mm Audio Output (PCM5102 Audio Codec @44100Hz 32-bit)
* 4 Eurorack-Compatible Trigger Inputs
* 2 CV Inputs
* USB Type-C for MCU USB
* USB Type-C for UART Debug Bridges
* Mount for I2C 128x32 SSD1306 OLED
* Mount for PEC12R Encoder with Button
* User/Trigger Button

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

## Development

The pipeline will trigger a full build on Push or Pull Request.

### Releasing

The pipeline will trigger a new release build on following tagging scheme:

```
git tag -a v1.0.0 -m "Release v1.0.0"
git push origin v1.0.0
```
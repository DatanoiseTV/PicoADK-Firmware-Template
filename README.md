# PicoADK FreeRTOS Template (with VultDSP runtime and USB MIDI)

![PicoADK_Top](https://user-images.githubusercontent.com/6614616/204331473-485c4a37-0c32-4387-92df-22c15a2e22aa.jpg)

Photos courtesy of Paul D. Pape - derwellenreiter for schneidersladen, Berlin, Germany. 

This C++ boilerplate template allows for creating Synthesizers, Effects and other Audio DSP related projects. It targets the "PicoADK" board (link below), which I have created.
## Prerequisites

1. Install the [Vult](https://github.com/vult-dsp/vult/) compiler: `sudo npm install vult -g`

## Building 
```
git clone --recursive https://github.com/DatanoiseTV/PicoADK-FreeRTOS-Template picoadk-template
cd picoadk-template
mkdir build && cd build
cmake ..
make
```
Now you can find a main.uf2 in the build folder, which is your firmware.

## Copying the Firmware to the PicoADK

Plug in the PicoADK USB Type-C while holding the **BOOT** button **or** hold **BOOT** and press the reset button quickly.
After that, a **RPI-RP2** disk volume will appear. Simply drag and drop the UF2 file to this drive and the PicoADK will
reboot after a moment, the drive will disappear and your firmware will be running.

## More information

Please check the [Pico Getting Started Guide](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) on how to install the toolchain and required libraries for your OS.

## Development

The pipeline will trigger a full build on Push or Pull Request.

### Releasing

The pipeline will trigger a new release build on following tagging scheme:

```
git tag -a v1.0.0 -m "Release v1.0.0"
git push origin v1.0.0
```

# Hardware

The way-to-go hardware option is the [PicoADK](https://github.com/DatanoiseTV/PicoADK-Hardware)

# Getting RAM and flash usage statistics

![image](https://user-images.githubusercontent.com/6614616/203217505-7b235539-9cc0-42c1-a4ca-f910ef306fb1.png)

Your statically allocated memory and flash usage will be reported upon linking.

## Community

You can find the PicoADK at Discord right [here](https://discord.gg/gcwAGZ9e) and a community discussion board on [GitHub Discussions](https://github.com/DatanoiseTV/PicoADK-Hardware/discussions)

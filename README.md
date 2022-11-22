# PicoADK FreeRTOS Template (with VultDSP runtime and USB MIDI)
![PicoADK Render](https://user-images.githubusercontent.com/6614616/203218532-9e1f4f0e-e546-4674-9c91-a76579b1532a.png)


This C++ boilerplate template allows for creating Synthesizers, Effects and other Audio DSP related projects based on the cheap and easily available RP2040 MCU. It targets the "PicoADK" board (link below), which I have created.
## Prerequisites

1. Install the [Vult](https://github.com/vult-dsp/vult/) compiler: `sudo npm install vult -g`

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

# Hardware

The currently best hardware option is the [PicoADK board](https://github.com/DatanoiseTV/PicoDSP-Hardware)

![PicoADK Board](https://user-images.githubusercontent.com/6614616/202743141-287a1278-63dd-40a0-bf19-9c3cd2b5591f.jpg)


# Getting RAM and flash usage statistics

![image](https://user-images.githubusercontent.com/6614616/203217505-7b235539-9cc0-42c1-a4ca-f910ef306fb1.png)

Your statically allocated memory and flash usage will be reported upon linking.

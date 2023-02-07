# Index

![PicoADK v1.1a Top](https://user-images.githubusercontent.com/6614616/208942839-973d7aec-1588-49ae-b695-3c5a1ed805d5.jpg)


## Introduction
The PicoADK is a RP2040 based Audio Development Kit, which allows you to build your own digital oscillators, synthesizers, noise boxes and experiment around. It has all the base features of the Raspberry Pico, plus a high quality Audio Output, 8 Analog Inputs for connecting potentiometers, control voltage from eurorack systems or even additional input signals.

Even though it is very small, it is capable of synthesizing and outputting a very high quality sound output.
Thanks to the Vult DSP language you can even create digital resonant filters, oscillators and even complete synthesizers!

## Technical Specs

* RP2040 Dual Core Cortex M0+ running at up to 400MHz
* USB Type-C connector
* 2MB Flash for Firmware and Data
* Very low noise LDO regulators (separate LDO for digital circuits and separate for analog circuits), no switching regulators
* Partially pin-compatible with RP2040 (besides a few pins internally used or rearranged), extra 4 pins on each side
* Audio DAC PCM5100A, up to 32-bit output
* Very high audio signal quality and very low noise
* Dedicated Boot and Reset Buttons
* 8 channel 12-bit ADC with up to 1MS/s, with selectable 3.3V range (on-board low-noise power supply) or range up to 5V via external VREF
* Low-Pass filter on each ADC input (fMax=48 KHz) at low-impedance (100 Ohm). With 5V VREF suitable for CV (no overvoltage protection, unipolar)
* Symbols marking special pin functions on the pin headers
* 4 LEDs on shared GPIO2-5 for debugging
* ESD Protection on USB
* SWD Debug Port

For pinout and technical details go [here](hardware_reference/) and if you want to get started with the PicoADK head over to [**Getting Started**](getting_started/).

## Media

You can find some sound teasers at [Soundcloud](https://soundcloud.com/datanoisebln).

[![PicoADK Monosynth Demo](http://img.youtube.com/vi/8SXC7bpoYGg/0.jpg)](http://www.youtube.com/watch?v=8SXC7bpoYGg "PicoADK Monosynth Demo")
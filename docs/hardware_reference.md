# Hardware Reference

## Pinout

![image](https://user-images.githubusercontent.com/6614616/198907086-aaeb9831-ceb2-4acc-8242-45671ba2a3fd.png)

Interactive pinout at [https://datanoise.net/picoadk/](https://datanoise.net/picoadk/)

## Internal Signals

Some signals are routed to internal peripherals as following:

| GPIO          | Function              | Shared w/ Header?   |
| ------------- | --------------------- | ------------------- |
| GPIO2         | Debug LED 1           | yes                 |
| GPIO3         | Debug LED 2           | yes                 |
| GPIO4         | Debug LED 3           | yes                 |
| GPIO5         | Debug LED 4           | yes                 |
| GPIO10        | ADC128 SPI1 SCK       | yes                 |
| GPIO11        | ADC128 SPI1 MOSI      | yes                 |
| GPIO12        | ADC128 SPI1 MISO      | yes                 |
| GPIO13        | ADC128 SPI1 CSn       | yes                 |
| GPIO15        | Big Debug LED (Ws2812 on v1.1a) | no                  |
| GPIO16        | PCM5100A I2S DAT      | no                  |
| GPIO17        | PCM5100A I2S BCLK     | no                  |
| GPIO18        | PCM5100A I2S LRCLK    | no                  |
| GPIO23        | PCM5100A DEMP         | no                  |
| GPIO24        | VBUS Detect Pin       | no                  |
| GPIO25        | PCM5102 XSMT          | no                  |


The SPI ADC shares the GPIO10 to GPIO13, which are also present on the pin headers. Please be aware of that.

Make sure your software controls the XSMT and DEMP pins. The XSMT needs to be controled by the software to mute/unmute the audio output.
With the DEMP pin you can control the Deemphasis for 44100 Hz.

## 5V ADC FAQ

By default, the ADC range is approximately 0-3.3v provided by a low-noise internal 3.3V voltage source.
If you want to use 0-5V input range, you need to cut the trace on the jumper on the bottom of the PCB and bridge the VEXT position with the middle of the jumper. MAKE SURE THAT YOU CHECK FOR NON-CONTINUITY WITH A MULTIMETER between the 3.3V reference side and the middle of the jumper to make sure you've cut the bridge properly. The recommended way of cutting the trace is using a sharp blade such as an exacto knife. Use of a dremel or similar is not recommended, as it could damage the inner layers of the board

## Potentiometer FAQ

The on board LDO is only able to supply around 100mA of current on the 3.3V output. Therefore, if you are using potentiometers where the CW side is attached to GND and the CCW side is attached to 3.3V, a big current draw might destroy your boards LDO regulator.

*Please* consider using an external 3.3V LDO if using the ADC in 3.3V mode or modify the solder jumper on the bottom of the PCB to allow the ADC to work with up to 5V range and potentiometers to be powered by 5V. Please see the remark above at *Changes required for 5V ADC range (default is 3.3v)*.

The most common external LDO for 3.3V operation is the AMS1117-3.3. The input of the external LDO should be attached to the VSYS pin or VBUS pin, depening on your requirements and should be decoupled with a capacitor on both input and output.

It is also recommended to connect each ADC pin with a big resistor (eg. 1M) to GND to avoid noise / floating inputs.
The inputs of the ADC all have a passive low-pass filter with a cutoff frequency of 48kHz at 100 Ohms load.

*Update as of "A" revision*: The LDO can supply 300mA now.

## Block Diagram
![PicoADK Diagram(1)](https://user-images.githubusercontent.com/6614616/199276802-3cfb1608-071d-42e8-8c82-e9a6716adb66.png)

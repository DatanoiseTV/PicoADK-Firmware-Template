# Installing on Windows

1. Press the Windows key
1. Type 'Powershell'
1. Right click 'Windows Powershell' 
1. Click 'Run as Administrator'
1. Type ```wsl --install```. This is the "[Windows Subsystem for Linux](https://learn.microsoft.com/en-us/windows/wsl/about)" needed to use Linux commands later in this installation
1. Press Enter
1. Wait for install to complete
1. Restart your computer
1. Re-open Windows Powershell using steps 1-4
1. Type ```wsl``` and press Enter
1. Wait for the new Ubuntu window to finish installing
1. 14. Create a username and password
1. Close all Powershell and Ubuntu windows
1. Press the Windows key and type 'Ubuntu', then click 'Ubuntu'
1. Type ```sudo apt install npm``` and press Enter. This is the [NPM](https://www.npmjs.com/) package library installer
1. Type ```y``` and press Enter when/if prompted
1. Wait for download to complete
1. Type ```sudo npm install vult -g``` and press Enter. This is the [Vult compiler](https://github.com/vult-dsp/vult/)
1. Type your password and press Enter when/if prompted
1. Type ```git clone --recursive https://github.com/DatanoiseTV/PicoADK-FreeRTOS-Template picoadk-template``` and press Enter
1. Type ```cd /``` and press Enter
1. Type ```cd mnt/c``` and press Enter
1. You are now in your computer's C drive. Use 'cd filepath', replacing the word filepath with the actual path to your 'picoadk-template' folder. For example: ```cd users/johnsmith/documents/picoadk-template```
1. Type ```cd picoadk-template``` and press Enter
1. Type ```export PICO_SDK_FETCH_FROM_GIT=1``` and press Enter
1. Type ```mkdir build && cd build``` and press Enter
1. Type ```sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential``` and press Enter
1. Type ```y``` and press Enter when/if prompted
1. Wait for download to complete
1. Type ```cmake ..``` and press Enter
1. Wait while it downloads, it may take a long time
1. Type ```make``` and press Enter
1. Wait for build to complete. This may take a while

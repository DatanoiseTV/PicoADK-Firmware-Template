# Installing on Windows

1. Press the Windows key
2. Type 'Powershell'
3. Right click 'Windows Powershell' 
4. Click 'Run as Administrator'
5. Type ```wsl --install```
6. Press Enter
7. Wait for install to complete
8. Restart your computer
9. Re-open Windows Powershell using steps 1-4
10. Type ```wsl```
11. Wait for the new Ubuntu window to finish installing
12. Create a username and password
13. Type ```cd mnt/c```
14. You are now in your computer's C drive. Use 'cd filepath', replacing the word filepath with the actual path to your 'picoadk-template' folder. For example: ```cd users/johnsmith/documents/picoadk-template```
15. Type ```git clone --recursive https://github.com/DatanoiseTV/PicoADK-FreeRTOS-Template picoadk-template``` and press Enter
16. Type ```cd picoadk-template``` and press Enter
17. Type ```export PICO_SDK_FETCH_FROM_GIT=1``` and press Enter
18. Type ```mkdir build && cd build``` and press Enter
19. Type ```sudo cmake ..```
20. Wait while it downloads, it may take a long time
21. Type ```make```

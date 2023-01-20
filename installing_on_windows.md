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
10. Type ```wsl``` and press Enter
11. Wait for the new Ubuntu window to finish installing
12. Create a username and password
13. Type ```sudo apt install npm``` and press Enter
14. Type ```y``` and press Enter when prompted
15. Wait for download to complete
16. Type ```sudo npm install vult -g``` and press Enter
17. Type your password and press Enter when/if prompted
18. Type ```cd mnt/c``` and press Enter
19. You are now in your computer's C drive. Use 'cd filepath', replacing the word filepath with the actual path to your 'picoadk-template' folder. For example: ```cd users/johnsmith/documents/picoadk-template```
20. Type ```git clone --recursive https://github.com/DatanoiseTV/PicoADK-FreeRTOS-Template picoadk-template``` and press Enter
21. Type ```cd picoadk-template``` and press Enter
22. Type ```export PICO_SDK_FETCH_FROM_GIT=1``` and press Enter
23. Type ```mkdir build && cd build``` and press Enter
24. Type ```sudo cmake ..``` and press Enter
25. Wait while it downloads, it may take a long time
26. Type ```make``` and press Enter

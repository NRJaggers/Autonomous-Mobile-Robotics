#! /bin/bash

#Run from WSL - Program to connect board and provide permissions
powershell.exe -File ./attach.ps1
lsusb
# make sure to change password below
echo <password> | sudo -S chmod +777 /dev/ttyACM0
echo -e "\n"
echo "Device connected and permissions granted"
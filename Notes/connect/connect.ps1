#Run from powershell - Program to connect board and provide permissions
Write-Host "Listing USB Connections"
usbipd wsl list
Write-Host "Mounting USB Connection to WSL"
usbipd wsl attach --busid 1-2 # change this to connection board usually is
Write-Host "Starting WSL and giving permissions"
sleep(1)
#make sure to change password below
c:\Windows\System32\bash.exe -c "lsusb; echo <password> | sudo -S chmod +777 /dev/ttyACM0"
exit
#Script that WSL can call to mount usb connection to WSL
Write-Host "Listing USB Connections"
usbipd wsl list
Write-Host "Mounting USB Connection to WSL"
usbipd wsl attach --busid 1-2 # change this to connection board usually is
sleep(1)
exit
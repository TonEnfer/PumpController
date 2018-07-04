@echo off
for %%i in ( "%cd%" ) do set "curdir1=%%~nxi"
cd ..
for %%i in ( "%cd%" ) do set "curdir=%%~nxi"
cd "%curdir1%"
avr-objcopy -O binary -I ihex "%curdir%".hex FLASH.bin
avr-objcopy -O binary -I ihex "%curdir%".eep EEPROM.bin

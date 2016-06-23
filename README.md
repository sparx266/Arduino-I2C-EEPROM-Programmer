# Arduino-I2C-EEPROM-Programmer
Quick and dirty 24LC256 &amp; 24LC512 programmer

Realising I needed to program some 24LC256 & 24LC512 I2C EEPROMS for some projects, there seemed to be no public programmers using an Arduino.  Yet so many Arduino text books suggest using these chips.

Here is a very rough but working programmer for these chips.
what you will need:

Arduino
I2C LCD (16 x 2 is probably easiest)
PS2 style joystick
SD card breakout board
SD card

.bin or .BIN files on card

All of these are easily available for cheap.

It is assumed you know how to connect the display and chip to the Arduino. If not research Arduino I2C.
The joystick connects to Gnd, +5V, A0, A1 & A2.
SD board connects to digital 10, 11, 12 & 13.
If you need to buy one, get one with a built in regulator, it should have a switch saying 3.3V and 5V. You need to switch it to 5V for MOST Arduinos including the Uno, Nano and Mega.
this is because an SDcard is 3.3V and Arduinos are 5V devices.  Connection details are given in the listing.

The program will detect the chip at address 0x50 and the LCD at 0x27. Change these if needed.
it will then look for the SD card and report an error if not found.
it will then look for .bin or .BIN files, binary files. It will list all those found, then report the number found.
there can be a maximum of 20 .bins's on a card.
it will then ask which chip you want programmed.  Left and right to select, press button to select.
next, choose file to load, press button to select.

It will start writing straight away. A 256 will take about 20 minutes to program and verify.  512 is untested at this time.

The verify routine isn't great, it will report a pass and and error on Gail. If no error then it passed!

This project is given freely in the hope it will help others learn or create something cool, amazing, clever, interesting or beneficial in some way.

You are free to improve the code in any way and if it proves useful, just say thanks!

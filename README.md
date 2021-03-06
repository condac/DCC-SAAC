# DCC-SAAC
DCC Stand Alone Arduino Controller

This project is focused on simple and easy to get up and running. And hopefully provide the basics needed to control the small scale trainlayouts. The Arduino is working by it self and do not need a computer to tell it what to do. 
The project use the same hardware as the DCC++ project making it a easy upgrade when you want to take the step to more advanced layouts and control with a computer attached. ( Or if you want to downgrade and not have the computer )

# DCC Support
For the moment only 128 speed step decoders and 12 Functions

# Märklin Motorola protocol 
Basic support for Märklins early protocol used in the DELTA series and early locomotives.
* 80 id numbers
* Light function
* Simultaneous running with DCC on same track !!!

The addresses used in DELTA is as following:
* 24, "Electric"
* 60, "Railcar"
* 72, "Diesel"
* 78, "Steam"

# Hardware
* Arduino Uno or Mega
* Arduino Motor Shield R3
* LCD Keypad shield 16x2
* 12-18v Power supply for rail track


## Optional hardware
* USB charger as power to arduino if you use more than 12V on rail tracks.

# Installation
You need to cut the brake pins on the motor shield and the current sensing on A0 (A channel) or they will interfer with with the LCD.

Connect track to channel B. 

If you have 12V power supply you can connect it directly to the arduino power plug and it will give power to everything. The voltage regulator on the arduino located between the USB and Power connector will get hot! But if you have more than 12V the Arduino will break! (I have tried... RIP my little Uno) 

If you have more than 12V you must cut the power line on the motor shield the (same way you cut the brake pins) and supply the motor sheild with the power. And then use a seperate power supply for the arduino. The easiest way is to use a cellphone charger with removable USB cable and connect the one you have for the arduino and power it through USB

## Configure locomotives
Before you hit upload on the arduino code you need to change id numbers and names for the locomotives you want to be able to use. See example: 
~~~~   
Train trains[MAX_TRAINS] {Train( 1, "RC1    ", UNCONF), 
                  Train( 3, "RC2    ", DCC), 
                  Train( 72, "ST    ", UNCONF), 
                  Train( 24, "Electric", UNCONF), //0220
                  Train( 60, "Railcar", UNCONF),  //0202
                  Train( 72, "Diesel", MOTOROLA),   //0022
                  Train( 78, "Steam", MOTOROLA),    //0222
                  Train( 80, "Delta Pilot", UNCONF), 
                  Train( 0, "noname9", UNCONF), 
                  Train( 0, "noname10", UNCONF), 

                  }; 
~~~~
The last parameter is set to DCC or MOTOROLA depending on the decoder format, set it to UNCONF for the trains you do not use. 

# CV Programming
A simple interface for CV programming exists. 

CAUTION!!! CV is sent to broadcast address so all train on track will get programmed!

Make sure you understand the risks with CV programming before use, more details on the dangers of programming on non protected mains http://www.dccwiki.com/Programming_Track

# Other
reference for dcc protocol https://www.nmra.org/sites/default/files/s-92-2004-07.pdf https://www.nmra.org/sites/default/files/s-9.2.1_2012_07.pdf

## Donations
Feel free to make any donations on my patreon page https://www.patreon.com/condac or with paypal https://www.paypal.me/condac

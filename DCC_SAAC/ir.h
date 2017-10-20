/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  IRL Receive

  Receives IR signals from different protocols and prints them to the Serial monitor.
  Choose your protocols that should be decoded. Remove the not used ones to save flash/ram/speed.
  You can choose a custom debounce time to not trigger a button two times in a row too fast.

  The following pins are usable for PinInterrupt or PinChangeInterrupt*:
  Arduino Uno/Nano/Mini: All pins are usable
  Arduino Mega: 10, 11, 12, 13, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64),
              A11 (65), A12 (66), A13 (67), A14 (68), A15 (69)
  Arduino Leonardo/Micro: 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI)
  HoodLoader2: All (broken out 1-7) pins are usable
  Attiny 24/44/84: All pins are usable
  Attiny 25/45/85: All pins are usable
  Attiny 13: All pins are usable
  Attiny 441/841: All pins are usable
  ATmega644P/ATmega1284P: All pins are usable

  PinChangeInterrupts* requires a special library which can be downloaded here: Or arduino builtin library browser
  https://github.com/NicoHood/PinChangeInterrupt
*/

// include PinChangeInterrupt library* BEFORE IRLremote to acces more pins if needed
#include "PinChangeInterrupt.h"

#include "IRLremote.h"

// Choose a valid PinInterrupt or PinChangeInterrupt* pin of your Arduino board
#define pinIR A3

// Choose the IR protocol of your remote. See the other example for this.
CNec IRLremote;
//CPanasonic IRLremote;
//CHashIR IRLremote;
//#define IRLremote Sony12

int lastButton = 0;

void initIR() {
  if (!IRLremote.begin(pinIR))
    Serial.println(F("You did not choose a valid pin for IR remote."));

  
}

void irLoop() {
  if (IRLremote.available())  {


    // Get the new data from the remote
    auto data = IRLremote.read();

    // Print the protocol data
    Serial.print(F("IR: "));
    Serial.print(data.address);
    Serial.print(F(" "));
    Serial.println(data.command);


/* pseudo
 *  
 *  if command then active command
 *  if address FFFF repeat last command
 */
    
    if (data.command == 30) {
      btnPressed = btnRIGHT;
      
    }
    if (data.command == 10) {
      btnPressed = btnLEFT;
    }
    if (data.command == 5) {
      btnPressed = btnUP;
    }
    if (data.command == 2) {
      btnPressed = btnDOWN;
    }
    if (data.command == 64) {
      btnPressed =  btnSELECT;
    }

    if (data.command == 77) { // POWER OFF BUTTON
      trackPower = false;
    }
    if (data.command == 22) { // POWER ON BUTTON
      trackPower = true;
    }
    
    if (data.command != 0) {
      lastButton = btnPressed;
      longPressTime = MIN_BUTTON_TRIGGER*2;
    }
    if (data.address == 65535) {
      btnHold = lastButton;
      longPressTime += MIN_BUTTON_TRIGGER*2;
    }
  }
}


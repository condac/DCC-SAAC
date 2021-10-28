// External includes
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#define UNCONF 0
#define MOTOROLA 3
#define DCC 4

// Macros to support option testing
#define _CAT(a, ...) a ## __VA_ARGS__
#define SWITCH_ENABLED_false 0
#define SWITCH_ENABLED_true  1
#define SWITCH_ENABLED_0     0
#define SWITCH_ENABLED_1     1
#define SWITCH_ENABLED_      1
#define ENABLED(b) _CAT(SWITCH_ENABLED_, b)
#define DISABLED(b) (!_CAT(SWITCH_ENABLED_, b))
//#define REMOTE(board) (MOTHERBOARD==BOARD_##board)

// Internal include
#include "Train.h"
#include "Configuration.h"
#include "common.h"

#include "buttons.h"

#if ENABLED(IRSUPPORT)
  #include "ir.h"
#endif

#if ENABLED(USE_TM1638_KEYPAD)
  #include "keypad.h"
#endif

int trainCounter = 0;
int current_packet = DCC;
bool packageReady = false;
int currentDraw;


void setup() {
  
  dcc_init();
  delta_init();
  timer_init();
  initLCD();
  Serial.begin(115200);
//  trains[0].setSpeed(1);
#if ENABLED(IRSUPPORT)
  initIR();
#endif
#if ENABLED(USE_TM1638_KEYPAD)
  setupKeypad();
#endif
}
 
void loop() {
  //lcdLoop();
  //unsigned long hej = millis();

  //Serial.println(arbitraryBaseRight( 21, 3, 3));
  /*
  Serial.print(" btnPressed: ");
  Serial.print(btnPressed);
  Serial.print(" btnHold: ");
  Serial.print(btnHold);
  Serial.print(" longPressTime: ");
  Serial.println(longPressTime);
  */
  btn_loop();
  
  #if ENABLED(IRSUPPORT)
  irLoop();
  #endif

  #if ENABLED(USE_TM1638_KEYPAD)
  keypadLoop();
  #endif

  drawMenu();
  
  currentWatch();
  
  checkTrackPower();
  //unsigned long hejend =  millis();
  //Serial.println(hejend-hej);
  //packNextMessage();
  
}
void checkTrackPower() {
  if (trackPower) {
    digitalWrite(DCC_POWER, HIGH);
  }
  else {
    digitalWrite(DCC_POWER, LOW);
  }
}

void currentWatch() {
  long tempcurrent = analogRead(A1);
  currentDraw = (currentDraw*SHORT_SMOOTHING + tempcurrent )/(SHORT_SMOOTHING+1);
  
  //Serial.println(currentDraw);
  
  
  if (currentDraw > CURRENT_LIMIT) {
    Serial.println("Over current protection triggered!");
    digitalWrite(DCC_POWER, LOW);
    trackPower = false;
  }
  macurrent = (macurrent*DISPLAY_CURRENT_SMOOTHING + (currentDraw*3) )/(DISPLAY_CURRENT_SMOOTHING+1);
}

void packNextMessage() {

    if ( programingMode ) {
      current_packet = DCC;
      if ( programingModeSendData ) {
        trackPower = true; // make sure power is on
        digitalWrite(DCC_POWER, HIGH);
        buildCVPacket();
        programingModeSendData = false;
        packageReady = true;
        TCNT2=227; // from PULSE_DCC_NEW_TRUE
      }else {
        buildIdlePacket();
        packageReady = true;
        TCNT2=227; // from PULSE_DCC_NEW_TRUE
      }
      
    } 
    else {
      while (!trains[trainCounter].isConfigured()) {
        trainCounter++;
        if (trainCounter>=MAX_TRAINS) {
          trainCounter = 0;
        }
      }
      if (trains[trainCounter].isConfigured()) {
        if ( trains[trainCounter].getFormat() == DCC ) {
          //Serial.println("making new pakage DCC");
          current_packet = DCC;
          dcc_packMsg();
          packageReady = true;
          TCNT2=227; // from PULSE_DCC_NEW_TRUE
          
        } else if ( trains[trainCounter].getFormat() == MOTOROLA ) {
          //Serial.println("making new pakage Motorola");
          current_packet = MOTOROLA;
          //digitalWrite(DCC_DIR,HIGH);
          motorolaPackMsg();
          digitalWrite(DCC_DIR,LOW);
          //delayMicroseconds(4000);
          //bitStage = BIT_STAGE_LONG_SLEEP;
          //Serial.println("making new pakage Motorola");
          
          packageReady = true;
          digitalWrite(DCC_DIR,LOW);
          TCNT2=0;
        }
      }
    
      trainCounter++;
      if (trainCounter>=MAX_TRAINS) {
        trainCounter = 0;
      }
    }

}

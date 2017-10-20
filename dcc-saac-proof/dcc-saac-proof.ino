// External includes
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#define UNCONF 0
#define MOTOROLA 3
#define DCC 4

// Internal include
#include "Train.h"

#define DCC_POWER   11
#define DCC_DIR     13



bool trackPower = false;
int trainCounter = 0;
int current_packet = DCC;
bool packageReady = false;

#define MAX_TRAINS 10 // with 10 trains the performance is about 2-3 updates/s

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

int currentDraw;


void setup() {
  
  dcc_init();
  delta_init();
  timer_init();
  initLCD();
  Serial.begin(115200);
//  trains[0].setSpeed(1);
}
 
void loop() {
  //lcdLoop();
  //unsigned long hej = millis();

  //Serial.println(arbitraryBaseRight( 21, 3, 3));
  
  btn_loop();
  
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

  currentDraw = analogRead(A1);
  //Serial.println(currentDraw);
  if (currentDraw > 660) {
    Serial.println("Over current protection triggered!");
    digitalWrite(DCC_POWER, LOW);
    trackPower = false;
  }
}

void packNextMessage() {

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
        delayMicroseconds(4000);
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


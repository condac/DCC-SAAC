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
                  Train( 24, "Electric", MOTOROLA), 
                  Train( 60, "Railcar", MOTOROLA), 
                  Train( 72, "Diesel", MOTOROLA), 
                  Train( 78, "Steam", MOTOROLA), 
                  Train( 80, "Delta Pilot", MOTOROLA), 
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
  Serial.println("Buttons");
  btn_loop();
  Serial.println("Menu");
  drawMenu();
  Serial.println("current");
  currentWatch();
  Serial.println("trackpower");
  checkTrackPower();
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
  if (!packageReady) { // package send is completed
    //while (!trains[trainCounter].isConfigured()) {
    //  trainCounter++;
    //  if (trainCounter>=MAX_TRAINS) {
    //    trainCounter = 0;
    //  }
    //}
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
        motorolaPackMsg();
        digitalWrite(DCC_DIR,LOW);
        delayMicroseconds(2100);
        //Serial.println("making new pakage Motorola");
        packageReady = true;
        digitalWrite(DCC_DIR,LOW);
        TCNT2=250;
      }
    }
  
  
    trainCounter++;
    if (trainCounter>=MAX_TRAINS) {
      trainCounter = 0;
    }
  }
  
}


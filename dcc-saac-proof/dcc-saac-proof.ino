// External includes
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Internal include
#include "Train.h"

#define DCC_POWER   11
#define DCC_DIR     13


Train dummy(1, "RC1");

#define MAX_TRAINS 10

Train trains[MAX_TRAINS] {Train( 1, "RC1"), 
                  Train( 2, "RC2"), 
                  Train( 3, "RC3"), 
                  Train( 0, "noname4"), 
                  Train( 0, "noname5"), 
                  Train( 0, "noname6"), 
                  Train( 0, "noname7"), 
                  Train( 0, "noname8"), 
                  Train( 0, "noname9"), 
                  Train( 0, "noname10"), 

                  };

int currentDraw;


void setup() {
  dcc_init();
  initLCD();
  Serial.begin(9600);
//  trains[0].setSpeed(1);
}
 
void loop() {
  //lcdLoop();
  btn_loop();
  dcc_packMsg();
  drawMenu();
  currentWatch();
}

void currentWatch() {

  currentDraw = analogRead(A1);
  //Serial.println(currentDraw);
  if (currentDraw > 660) {
    Serial.println("Over current protection triggered!");
    digitalWrite(DCC_POWER, LOW);
  }
}



#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


void setup() {
  initLCD();
  Serial.begin(9600);
}
 
void loop() {
  lcdLoop();
  

  get_buttons();

}


void initLCD() {

  lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("DCC-SAAC-PROOF"); // print a simple message
}


void lcdLoop() {

  lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
  lcd.print(millis()/1000);      // display seconds elapsed since power-up


  lcd.setCursor(0,1);            // move to the begining of the second line
  
}


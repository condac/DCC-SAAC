#define BUTTON_PIN A0

#define btnNONE   0
#define btnRIGHT  1
#define btnUP     2
#define btnDOWN   3
#define btnLEFT   4
#define btnSELECT 5

#define LONG_PRESS 2
#define SHORT_PRESS 1

#define MIN_BUTTON_TRIGGER 50 // The button must be press this many ms for a trigger to hapen


bool newButton = false;
int btnPressed = 0;
int btnHold = 0;
int lastKey = btnNONE;
unsigned long longPressTime = 0;
unsigned long startPressTime = 0;
unsigned long btnLoopTime = 0;

int read_LCD_buttons() {
 int adc_key_in  = 0;
 adc_key_in = analogRead(BUTTON_PIN);      // read the value from the sensor 
 //Serial.println(adc_key_in);
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 // For V1.0 comment the other threshold and use the one below:
/*
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
*/
 return btnNONE;  // when all others fail, return this...
}



void btn_loop() {
  // this loops at unknown speeds depending on program states. 
  // We need to keep track of what buttons are pressed,
  // and for how long to create short or long press events
  
  unsigned long timeToAdd = millis() - btnLoopTime;
  btnLoopTime = millis();
  unsigned long compareTime = btnLoopTime - startPressTime;
  
  int thisKey = read_LCD_buttons();
  
  if ( thisKey != lastKey ) {
    startPressTime = btnLoopTime;
    
    if ( compareTime > MIN_BUTTON_TRIGGER ) {
      Serial.println("Button trigger");
      btnPressed = lastKey;
      newButton = true;
    }
  }
  
  if ( (thisKey == lastKey) && (thisKey != btnNONE) ) {
    
    if (compareTime > MIN_BUTTON_TRIGGER && newButton) {
      Serial.println("Button reset timer");
      longPressTime = 0;
      newButton = false;
    }
    if ( compareTime > MIN_BUTTON_TRIGGER ) {
      btnHold = lastKey;
    }
    longPressTime += timeToAdd;
  } 
  
  

  lastKey = thisKey;
}



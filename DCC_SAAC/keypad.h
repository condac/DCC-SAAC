

#ifdef USE_TM1638_KEYPAD
#include <TM1638plus_Model2.h>
// GPIO I/O pins on the Arduino connected to strobe, clock, data, pick on any I/O pin you want.
bool swap_nibbles = false; //Default is false if left out, see note in readme at URL
bool high_freq = false; //default false,, If using a high freq CPU > ~100 MHZ set to true. 

// Constructor object
TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, swap_nibbles, high_freq);

unsigned char kp_buttonPressed;

#define kpB_1 2
#define kpB_2 3
#define kpB_3 4
#define kpB_4 6
#define kpB_5 7
#define kpB_6 8
#define kpB_7 10
#define kpB_8 11
#define kpB_9 12
#define kpB_0 15
#define kpB_A 1
#define kpB_B 5
#define kpB_C 9
#define kpB_D 13
#define kpB_T1 14
#define kpB_T2 16

int kp_currentMenu = 0;
#define KP_MENU_DEFAULT 0
#define KP_MENU_SELECT_T 2


#define THROTTLE_SMOOTH 10
#define THROTTLE_DEADBAND 50
long currentThrottle1;
long currentThrottle2;

int prevSpeed1;
int prevSpeed2;

int selectedT1 = -1;
int selectedT2 = -1;
int selectedSlot;

char decMask = 0;
char tm_buffer[10];
char tm_message[10];
long displayMessage;

#define kp_DISPLAY_MESSAGE_TIME 4000
#define kp_DISPLAY_SPEED_TIME 2000

long displaySpeed1;
long displaySpeed2;

#define kp_DISPLAY_DIFF 5
int prevDispSpeed1;
int prevDispSpeed2;

int kp_buttonTime = 0;
int kp_buttonPrev = 0;
bool kp_buttonHold = false;

long currenttime;

void setupKeypad() {
  tm.displayBegin(); // Init the module
  delay(10);
  tm.reset();
  selectedT1 = -1;
  selectedT2 = -1;
}

void throttleSmooth() {
  long tempcurrent = analogRead(THROTTLE_INPUT1);
  currentThrottle1 = (currentThrottle1*THROTTLE_SMOOTH + tempcurrent )/(THROTTLE_SMOOTH+1);
  tempcurrent = analogRead(THROTTLE_INPUT2);
  currentThrottle2 = (currentThrottle2*THROTTLE_SMOOTH + tempcurrent )/(THROTTLE_SMOOTH+1);
}

int analogToSpeed(int analog) {
  int out = 0;
  if (analog > 511) {
    out = map(analog, 512+THROTTLE_DEADBAND, 1010, 0, 127);
    out = constrain(out, 0,127);
  } else {
    out = map(analog, 512-THROTTLE_DEADBAND, 0, -1, -127);
    out = constrain(out, -127, -1);
  }
  
  return out; 
}
void checkThrottle() {
  int currentSpeed1 = analogToSpeed(currentThrottle1);
  int currentSpeed2 = analogToSpeed(currentThrottle2);
  if (currentSpeed1 != prevSpeed1) {
    prevSpeed1 = currentSpeed1;
    if (selectedT1 >=0 && selectedT1 < MAX_TRAINS) {
      trains[selectedT1].setSpeed(currentSpeed1);
      //displaySpeed1 = millis()+kp_DISPLAY_SPEED_TIME;
    }
  }
  
  if (currentSpeed2 != prevSpeed2) {
    prevSpeed2 = currentSpeed2;
    if (selectedT2 >=0 && selectedT2 < MAX_TRAINS) {
      trains[selectedT2].setSpeed(currentSpeed2);
      //displaySpeed2 = millis()+kp_DISPLAY_SPEED_TIME;
    }
  }

  if (currentSpeed1-kp_DISPLAY_DIFF > prevDispSpeed1 || currentSpeed1+kp_DISPLAY_DIFF < prevDispSpeed1) {
    prevDispSpeed1 = currentSpeed1;
    if (selectedT1 >=0 && selectedT1 < MAX_TRAINS) {
      displaySpeed1 = millis()+kp_DISPLAY_SPEED_TIME;
    }
  }
  if (currentSpeed2-kp_DISPLAY_DIFF > prevDispSpeed2 || currentSpeed2+kp_DISPLAY_DIFF < prevDispSpeed2) {
    prevDispSpeed2 = currentSpeed2;
    if (selectedT2 >=0 && selectedT2 < MAX_TRAINS) {
      displaySpeed2 = millis()+kp_DISPLAY_SPEED_TIME;
    }
  }
}


bool drawMessage() {
  currenttime = millis();
  

  if (selectedSlot == 1) {
    decMask = 0b11110000;
  } else if (selectedSlot == 2) {
    decMask = 0b00001111;
  } 
  

  if (displayMessage > currenttime) {
    tm.DisplayStr(tm_message, decMask);
    //snprintf(tm_buffer,9, "T1  %4d", prevSpeed1);
    return true;
  } 
  else {
    return false;
  }
  
  
  //tm.DisplayStr(tm_message, decMask);
  return true;
}

int calculateButton() {
  int butt = tm.ReadKey16();

  if (butt >0) {
    if (kp_buttonPrev != butt) {
      kp_buttonTime = 0;
      kp_buttonPrev = butt;
    } else {
      kp_buttonTime++;
      if (kp_buttonTime >= 2) {
        if (kp_buttonHold) {
          return 0;
        }
        kp_buttonHold = true;
        return butt;
      }
    }
  }
  else {
    kp_buttonTime = 0;
    kp_buttonPrev = butt;
    kp_buttonHold = false;
  }
  return 0;
}

void kpMenuDefault() {
  static char selectedPressed;
  static char selectedPressedPrev;
  kp_buttonPressed = calculateButton();

  if (kp_buttonPressed == kpB_T1) {
    selectedSlot = 1;
  }
  if (kp_buttonPressed == kpB_T2) {
    selectedSlot = 2;
  }

  if (kp_buttonPressed == kpB_T1 || kp_buttonPressed == kpB_T2) {
    if (kp_buttonPressed == selectedPressedPrev) {
      selectedPressed++;
      if (selectedPressed > 3) {
        kp_currentMenu = KP_MENU_SELECT_T;
        
        selectedPressed = 0;
      }
    }else {
      selectedPressedPrev = kp_buttonPressed;
    }
  }
  int currentSelected = 0;
  if (selectedSlot == 1) {
    currentSelected = selectedT1;
  } 
  else if(selectedSlot == 2) {
    currentSelected = selectedT2;
  }
  if (currentSelected >= 0 && currentSelected < MAX_TRAINS) {
    if (kp_buttonPressed == kpB_1) {
      trains[currentSelected].toggleFunction(1);
    }
    if (kp_buttonPressed == kpB_2) {
      trains[currentSelected].toggleFunction(2);
    }
    if (kp_buttonPressed == kpB_3) {
      trains[currentSelected].toggleFunction(3);
    }
    if (kp_buttonPressed == kpB_4) {
      trains[currentSelected].toggleFunction(4);
    }
    if (kp_buttonPressed == kpB_5) {
      trains[currentSelected].toggleFunction(5);
    }
    if (kp_buttonPressed == kpB_6) {
      trains[currentSelected].toggleFunction(6);
    }
    if (kp_buttonPressed == kpB_7) {
      trains[currentSelected].toggleFunction(7);
    }
    if (kp_buttonPressed == kpB_8) {
      trains[currentSelected].toggleFunction(8);
    }
    if (kp_buttonPressed == kpB_9) {
      trains[currentSelected].toggleFunction(9);
    }
    if (kp_buttonPressed == kpB_0) {
      trains[currentSelected].toggleFunction(0);
    }
  }

  if (kp_buttonPressed == kpB_D) { // Stop all trains
    for (int i = 0; i<MAX_TRAINS; i++) {
      trains[i].setSpeed(0);
    }
  }
  if ( kp_buttonPressed == kpB_A) {// Toggle power
    trackPower = !trackPower;
    
  }
  

  if (!drawMessage()) {
    //snprintf(tm_buffer,9, "%d  %4d", selectedT1+1, selectedT2+1);
    //tm.DisplayStr(tm_buffer, decMask);
    
    if (displaySpeed1 > currenttime && displaySpeed2 > currenttime) {
      snprintf(tm_buffer,9, "%4d%4d", prevSpeed1, prevSpeed2);
      
    }else if (displaySpeed1 > currenttime) {
      snprintf(tm_buffer,9, "%4d%4d", prevSpeed1, selectedT2+1);
    }  
    else if (displaySpeed2 > currenttime) {
      
      snprintf(tm_buffer,9, "%d   %4d", selectedT1+1, prevSpeed2);
    } 
    else {
      if (selectedT1+1 >99) {
        snprintf(tm_buffer,9, "%3d %4d", selectedT1+1, selectedT2+1);
      } else if (selectedT1+1 >9) {
        snprintf(tm_buffer,9, "%2d  %4d", selectedT1+1, selectedT2+1);
      } else {
        snprintf(tm_buffer,9, "%d   %4d", selectedT1+1, selectedT2+1);
      }
    }
    tm.DisplayStr(tm_buffer, decMask);
  } else {
    //snprintf(tm_buffer,9, "T1  %4d", prevSpeed1);
    //snprintf(tm_buffer,9, "%4d%4d", prevSpeed1, prevSpeed2);
   
  }
  
  
}

int kp_enteredNumber = 0;

void kpMenuSelectT() {
  
  kp_buttonPressed = calculateButton();
  
  
  switch (kp_buttonPressed) {
    case kpB_1: {
        kp_enteredNumber = kp_enteredNumber*10;
        kp_enteredNumber = kp_enteredNumber + 1;
        break;
    }
    case kpB_2: {
        kp_enteredNumber = kp_enteredNumber*10;
        kp_enteredNumber = kp_enteredNumber + 2;
        break;
    }
    case kpB_3: {
        kp_enteredNumber = kp_enteredNumber*10;
        kp_enteredNumber = kp_enteredNumber + 3;
        break;
    }
    case kpB_4: {
        kp_enteredNumber = kp_enteredNumber*10;
        kp_enteredNumber = kp_enteredNumber + 4;
        break;
    }
    case kpB_5: {
        kp_enteredNumber = kp_enteredNumber*10;
        kp_enteredNumber = kp_enteredNumber + 5;
        break;
    }
    case kpB_6: {
        kp_enteredNumber = kp_enteredNumber*10;
        kp_enteredNumber = kp_enteredNumber + 6;
        break;
    }
    case kpB_7: {
        kp_enteredNumber = kp_enteredNumber*10;
        kp_enteredNumber = kp_enteredNumber + 7;
        break;
    }
    case kpB_8: {
        kp_enteredNumber = kp_enteredNumber*10;
        kp_enteredNumber = kp_enteredNumber + 8;
        break;
    }
    case kpB_9: {
        kp_enteredNumber = kp_enteredNumber*10;
        kp_enteredNumber = kp_enteredNumber + 9;
        break;
    }
    case kpB_0: {
        kp_enteredNumber = kp_enteredNumber*10;
        kp_enteredNumber = kp_enteredNumber + 0;
        break;
    }
    case kpB_T1: {
        if (kp_buttonPressed == kpB_T1 && selectedSlot == 1) {
          selectedT1 = kp_enteredNumber - 1;
          kp_currentMenu = KP_MENU_DEFAULT;
          kp_enteredNumber = 0;
          if (!trains[selectedT1].isConfigured() ) {
            selectedT1 = -1;
            snprintf(tm_message,9, "ERR  404");
            displayMessage = millis() + kp_DISPLAY_MESSAGE_TIME;
          }
        }else {
          kp_currentMenu = KP_MENU_DEFAULT;
          kp_enteredNumber = 0;
        }
        break;
    }
    case kpB_T2: {
        if (kp_buttonPressed == kpB_T2 && selectedSlot == 2) {
          selectedT2 = kp_enteredNumber -1;
          kp_currentMenu = KP_MENU_DEFAULT;
          kp_enteredNumber = 0;
          if (!trains[selectedT2].isConfigured() ) {
            selectedT2 = -1;
            snprintf(tm_message,9, "ERR  404");
            displayMessage = millis() + kp_DISPLAY_MESSAGE_TIME;
          }
        }else {
          kp_currentMenu = KP_MENU_DEFAULT;
          kp_enteredNumber = 0;
        }
        break;
    }
    case 0: {
        
        break;
    }
    default: {
      kp_currentMenu = KP_MENU_DEFAULT;
      kp_enteredNumber = 0;
      break;
    }
    
    
  }

  
    long temp = millis()/100;
    if (temp % 2){
      decMask = 0;
    }
    snprintf(tm_buffer,9, "SEL %d_", kp_enteredNumber);
    tm.DisplayStr(tm_buffer, decMask);
  
}

void keypadLoop() {

  switch (kp_currentMenu) {
    case KP_MENU_DEFAULT: {
        kpMenuDefault();
        break;
    }
    case KP_MENU_SELECT_T: {
        kpMenuSelectT();
        break;
    }
  }
  

  #if ENABLED(USE_ANALOG_THROTTLES_WITH_KEYPAD)
    throttleSmooth();
    checkThrottle();
  #endif


}



#endif // USE_TM1638_KEYPAD

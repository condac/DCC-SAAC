/* Delta adresses
  * 24 - "Electric"
  * 60 - "Railcar"
  * 72 - "Diesel"
  * 78 - "Steam"
  * 80 - "Delta Pilot"

*/

/* times
 *  26µs short pulse
 *  182µs long pulse
 */

 /*
  * The motorola command uses triplets and not binary, but if you rethink it is a binary value
  * So what i have done is made the triplets in to 4 bits
  * 
  */
#define PULSE_MOTOROLA_SHORT 243 // 12
#define PULSE_MOTOROLA_LONG 165  // 90
#define PULSE_MOTOROLA_SLEEP 47  // 208


byte triplet = 0;
byte currentBit = 0;
byte bitStage = 0;
bool flip;
bool motoPackageReady = false;
int msgI = 0;

bool sendTwo = true;

const int messageLength = 26;
byte messagetest[messageLength] { 
                       0,0,
                       0,0,
                       1,0,
                       1,0,
                       1,1,
                       1,1,
                       0,0,
                       1,1,
                       1,1,
                       2,2,
                       2,2,
                       2,2,
                       2,2
                      };
                       
#define BIT_STAGE_BEGIN 1
#define BIT_STAGE_END 2
#define BIT_STAGE_DONE 3

void delta_init() {
  pinMode(DCC_DIR, OUTPUT);
  pinMode(DCC_POWER, OUTPUT);


  
}

void motorolaPackMsg() {

    //Serial.print("building Motorola");
  for (int i;i<26;i++) {
    messagetest[i] = 0;
  }
  byte mspeed = abs(trains[trainCounter].getSpeed()/9);
  if (mspeed == 1) {
    mspeed = 0;
  }
  if (trains[trainCounter].direction>0) {
    trains[trainCounter].direction--;
    mspeed = 1;
  }

  // address
  if (trains[trainCounter].getID() == 78) {
    messagetest[0] = 0;
    messagetest[1] = 0;

    messagetest[2] = 1;
    messagetest[3] = 0;

    messagetest[4] = 1;
    messagetest[5] = 0;

    messagetest[6] = 1;
    messagetest[7] = 0;
  }
  if (trains[trainCounter].getID() == 72) {
    messagetest[0] = 0;
    messagetest[1] = 0;

    messagetest[2] = 0;
    messagetest[3] = 0;

    messagetest[4] = 1;
    messagetest[5] = 0;

    messagetest[6] = 1;
    messagetest[7] = 0;
  }
  if (trains[trainCounter].getFunction(0)) {
    messagetest[8] = 1;
    messagetest[9] = 1;
  }
  // speed
  if (mspeed & 0b00000001) {
    messagetest[10] = 1;
    messagetest[11] = 1;
  }
  if (mspeed & 0b00000010) {
    messagetest[12] = 1;
    messagetest[13] = 1;
  }
  if (mspeed & 0b00000100) {
    messagetest[14] = 1;
    messagetest[15] = 1;
  }
  if (mspeed & 0b00001000) {
    messagetest[16] = 1;
    messagetest[17] = 1;
  }
  messagetest[18] = 2;
  messagetest[19] = 2;
  messagetest[20] = 2;
  messagetest[21] = 2;
  messagetest[22] = 2;
  messagetest[23] = 2;
  messagetest[24] = 2;
  messagetest[25] = 2;
  //Serial.println(".");
  //return this->moto;


    //byte* trainmsg = trains[trainCounter].getMotorola();
    //for (int i;i<messageLength;i++) {
    //  messagetest[i] = trainmsg[i];
    //}


}


  
void motorola_timer() {
  unsigned char latency;

  if (packageReady) {
    bitStage++;
    digitalWrite(DCC_DIR,flip);
  

    if (bitStage >=BIT_STAGE_DONE) {
      // move to next bit
      bitStage = BIT_STAGE_BEGIN;
      // currentBit = next bit in array to send
      
      currentBit = messagetest[msgI];
      msgI++;
      if (msgI >messageLength) {
        if (sendTwo) {
          sendTwo = false;
          msgI = 1;
          currentBit = messagetest[msgI];
          digitalWrite(DCC_DIR,HIGH);
          
        }else {
          packageReady = false; // prepare next message
          sendTwo = true;
          msgI = 0;
          currentBit = messagetest[msgI];
          digitalWrite(DCC_DIR,LOW);
          TCNT2=PULSE_MOTOROLA_LONG; 
          return;
        }
          
        
      }
      
    }
    
    if (currentBit == 1) {
      // send a true command
      if (bitStage == BIT_STAGE_BEGIN) {
        flip = false;
        latency= TCNT2;
        TCNT2=latency+PULSE_MOTOROLA_LONG; 
      } else {
        if (messagetest[msgI] == 2) {
          flip = false;
        }else {
          flip = true;
        }
        latency= TCNT2;
        TCNT2=latency+PULSE_MOTOROLA_SHORT; 
      }
    } else if (currentBit == 2) {
      if (bitStage == BIT_STAGE_BEGIN) {
        digitalWrite(DCC_DIR,LOW);
        flip = false;
        latency= TCNT2;
        TCNT2=latency+PULSE_MOTOROLA_LONG; 
      } else {
        digitalWrite(DCC_DIR,LOW);
        flip = false;
        latency= TCNT2;
        TCNT2=latency+PULSE_MOTOROLA_SHORT; 
      }
      
    }
    else {
      // send a false command
      if (bitStage == BIT_STAGE_BEGIN) {
        
        flip = false;
        latency= TCNT2;
        TCNT2=latency+PULSE_MOTOROLA_SHORT; 
      } else {
        if (messagetest[msgI] == 2) {
          flip = false;
        }else {
          flip = true;
        }
        
        latency= TCNT2;
        TCNT2=latency+PULSE_MOTOROLA_LONG; 
      }
    }
  }else {
    digitalWrite(DCC_DIR,LOW);
    TCNT2=PULSE_MOTOROLA_LONG;
  }
  
  
}


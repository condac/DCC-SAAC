#define PULSE_DCC_TRUE 142
#define PULSE_DCC_FALSE 28
#define PULSE_DCC_NEW_TRUE 227
#define PULSE_DCC_NEW_FALSE 198

unsigned char last_timer=PULSE_DCC_NEW_TRUE;  // store last timer value
   
unsigned char flag=0;  // used for short or long pulse
unsigned char every_second_isr = 0;  // pulse up or down

// definitions for state machine 
#define PREAMBLE 0    
#define SEPERATOR 1
#define SENDBYTE  2

unsigned char state= PREAMBLE;
unsigned char preamble_count = 16;
unsigned char outbyte = 0;
unsigned char bitmask = 0b10000000;

struct Message {
   unsigned char data[7];
   unsigned char len;
} ;

#define MAXMSG  5
// for the time being, use only two messages - the idle msg and the loco Speed msg

struct Message msg[MAXMSG] = { 
    { { 0xFF,     0, 0xFF, 0, 0, 0, 0}, 3},   // idle msg
    { { 0xFF, 0x3F,  0, 0, 0, 0, 0}, 4},   // locoMsg with 128 speed steps
    { { 0xFF, 0x80,  0, 0, 0, 0, 0}, 3}, // Function group 1
    { { 0xFF, 0xA0,  0, 0, 0, 0, 0}, 3}, // Function group 2
    { { 0xFF, 0xB0,  0, 0, 0, 0, 0}, 3} // Function group 3
  };               
                                
int msgIndex=0;  
int byteIndex=0;



bool dcc_flippflopp;

void dcc_init() {
  pinMode(DCC_DIR, OUTPUT);
  pinMode(DCC_POWER, OUTPUT);
/*
  // set up timer2 for 2mhz mode
  TCCR2A = 0;
  TCCR2B = 0<<CS22 | 1<<CS21 | 0<<CS20; 

  //Overflow Interrupt   
  TIMSK2 = 1<<TOIE2;

  // Start timer
  TCNT2=PULSE_DCC_NEW_TRUE; 
  */
}
void dcc_packMsg() {
  // debug performance

  byte xdata;
  if ( trains[trainCounter].isConfigured() ) {
    // Speed
    msg[1].data[0] = trains[trainCounter].getID();
    msg[1].data[1] = 0x3F;
    msg[1].data[2] = trains[trainCounter].getSpeedMsg();
    
    xdata = (msg[1].data[0] ^ msg[1].data[1]) ^ msg[1].data[2];
    msg[1].data[3] = xdata;
    
    // Function group 1
    msg[2].data[0] = trains[trainCounter].getID();
    msg[2].data[1] = trains[trainCounter].getFunctionGroup1();
    xdata = (msg[2].data[0] ^ msg[2].data[1]);
    msg[2].data[2] = xdata;

    // Function group 2
    msg[3].data[0] = trains[trainCounter].getID();
    msg[3].data[1] = trains[trainCounter].getFunctionGroup2();
    xdata = (msg[3].data[0] ^ msg[3].data[1]);
    msg[3].data[2] = xdata;

    // Function group 3
    msg[4].data[0] = trains[trainCounter].getID();
    msg[4].data[1] = trains[trainCounter].getFunctionGroup3();
    xdata = (msg[4].data[0] ^ msg[4].data[1]);
    msg[4].data[2] = xdata;
    
  } else {
    
  }

}

void dcc_old_timer() {
   unsigned char latency;
  
  // for every second interupt just toggle signal
  if (every_second_isr)  {
     digitalWrite(DCC_DIR,1);
     every_second_isr = 0;    
     
     // set timer to last value
     latency=TCNT2;
     TCNT2=latency+last_timer; 
     
  }  else  {  // != every second interrupt, advance bit or state
     digitalWrite(DCC_DIR,0);
     every_second_isr = 1; 
     
     switch(state)  {
       case PREAMBLE:
           flag=1; // short pulse
           preamble_count--;
           if (preamble_count == 0)  {  // advance to next state
              state = SEPERATOR;
              // get next message
              msgIndex++;
              if (msgIndex >= MAXMSG)  {  
                msgIndex = 0; 
                dcc_packMsg(); // prepare next train
              }  
              byteIndex = 0; //start msg with byte 0
           }
           break;
        case SEPERATOR:
           flag=0; // long pulse
           // then advance to next state
           state = SENDBYTE;
           // goto next byte ...
           bitmask = 0b10000000;  // send this bit next time first         
           outbyte = msg[msgIndex].data[byteIndex];
           break;
        case SENDBYTE:
           if (outbyte & bitmask)  { 
              flag = 1;  // send short pulse
           }  else  {
              flag = 0;  // send long pulse
           }
           bitmask = bitmask >> 1;
           if (bitmask == 0)  {  // last bit sent, is there a next byte?
              byteIndex++;
              if (byteIndex >= msg[msgIndex].len)  {
                 // this was already the XOR byte then advance to preamble
                 state = PREAMBLE;
                 preamble_count = 16;
                 
              }  else  {
                 // send separtor and advance to next byte
                 state = SEPERATOR ;
              }
           }
           break;
     }   
 
     if (flag)  {  // if data==1 then short pulse
        latency=TCNT2;
        TCNT2=latency+PULSE_DCC_TRUE;
        last_timer=PULSE_DCC_TRUE;
     }  else  {   // long pulse
        latency=TCNT2;
        TCNT2=latency+PULSE_DCC_FALSE; 
        last_timer=PULSE_DCC_FALSE;
     }  
  }
}

void dcc_new_timer() {
  unsigned char latency;
  
  // for every second interupt just toggle signal
  if (every_second_isr)  {
     digitalWrite(DCC_DIR,1);
     every_second_isr = 0;    
     
     // set timer to last value
     latency=TCNT2;
     TCNT2=latency+last_timer; 
     
  }  else  {  // != every second interrupt, advance bit or state
     digitalWrite(DCC_DIR,0);
     every_second_isr = 1; 
     
     switch(state)  {
       case PREAMBLE:
           flag=1; // short pulse
           preamble_count--;
           if (preamble_count == 0)  {  // advance to next state
              state = SEPERATOR;
              // get next message
              msgIndex++;
              if (msgIndex >= MAXMSG)  {  
                msgIndex = 0; 
                packageReady = false; //dcc_packMsg(); // prepare next train
              }  
              byteIndex = 0; //start msg with byte 0
           }
           break;
        case SEPERATOR:
           flag=0; // long pulse
           // then advance to next state
           state = SENDBYTE;
           // goto next byte ...
           bitmask = 0b10000000;  // send this bit next time first         
           outbyte = msg[msgIndex].data[byteIndex];
           break;
        case SENDBYTE:
           if (outbyte & bitmask)  { 
              flag = 1;  // send short pulse
           }  else  {
              flag = 0;  // send long pulse
           }
           bitmask = bitmask >> 1;
           if (bitmask == 0)  {  // last bit sent, is there a next byte?
              byteIndex++;
              if (byteIndex >= msg[msgIndex].len)  {
                 // this was already the XOR byte then advance to preamble
                 state = PREAMBLE;
                 preamble_count = 16;
                 
              }  else  {
                 // send separtor and advance to next byte
                 state = SEPERATOR ;
              }
           }
           break;
     }   
 
     if (flag)  {  // if data==1 then short pulse
        latency=TCNT2;
        TCNT2=latency+PULSE_DCC_NEW_TRUE;
        last_timer=PULSE_DCC_NEW_TRUE;
     }  else  {   // long pulse
        latency=TCNT2;
        TCNT2=latency+PULSE_DCC_NEW_FALSE; 
        last_timer=PULSE_DCC_NEW_FALSE;
     }  
  }
}



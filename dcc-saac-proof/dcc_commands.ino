#define PULSE_TRUE 143
#define PULSE_FALSE 33

int preamble_count = 20; // nmra states a minimum of 14 for a 6byte total message 20 preambles fill the data

char dccMessage[6]; // the data that we loop through and send. 

bool dcc_flippflopp;

void dcc_init() {
  pinMode(DCC_DIR, OUTPUT);
  pinMode(DCC_POWER, OUTPUT);

  // set up timer2 for 2mhz mode
  TCCR2A = 0;
  TCCR2B = 0<<CS22 | 1<<CS21 | 0<<CS20; 

  //Overflow Interrupt   
  TIMSK2 = 1<<TOIE2;

  // Start timer
  TCNT2=PULSE_FALSE; 
  
}
void dcc_packMsg() {

  
}

ISR(TIMER2_OVF_vect) {
  digitalWrite(DCC_DIR, dcc_flippflopp);
  dcc_flippflopp = !dcc_flippflopp;
  // start timer again
  TCNT2=PULSE_FALSE; 
}



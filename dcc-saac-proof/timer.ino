



void timer_init() {

    // set up timer2 for 0.5mhz mode
  TCCR2A = 0;
  TCCR2B = 0<<CS22 | 1<<CS21 | 1<<CS20; 

  //Overflow Interrupt   
  TIMSK2 = 1<<TOIE2;

  // Start timer
  TCNT2=PULSE_MOTOROLA_SHORT; 
}

ISR(TIMER2_OVF_vect) {
  //Serial.println(".");
  if (packageReady) {
    if (current_packet == MOTOROLA) {
      motorola_timer();
    } else {
      dcc_new_timer();
    }
  }else {
    noInterrupts();
    //Serial.println("pack");
    TCNT2=0;
    packNextMessage();
    //Serial.println("klar pack");
    interrupts();
    
  }
}


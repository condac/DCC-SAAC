
class Train {
  private:
    int speed;
    byte id;
    bool func[20];
    char name[9];
    bool configured; // true if the train is configured and usable
 
  public:
    Train(int idIn, String nameIn);
    void setSpeed(int inSpeed);
    int getSpeed();
    void toggleFunction(int funcNr);
    char* getName();
    byte getID();
    byte getSpeedMsg();
    bool isConfigured();
    byte getFunctionGroup1();
    byte getFunctionGroup2();
    byte getFunctionGroup3();
    bool getFunction(int funcNr);
};

Train::Train(int idIn, String nameIn) {
  // Constructor for a train
  if (idIn != 0) {
    this->configured = true;
    this->id = idIn;
    //this->name = nameIn;
    nameIn.toCharArray(this->name, 9);

    this->name[sizeof(this->name)-1] = 0;
    
  }
  
  this->speed = 0;
}



int Train::getSpeed() {
  return this->speed;
}

void Train::setSpeed(int inSpeed) {
  if(inSpeed >127) {
    inSpeed=127;
  }
  if(inSpeed < -126) {
    inSpeed=-126;
  }
  speed = inSpeed;
}

void Train::toggleFunction(int funcNr) {
  func[funcNr] = !func[funcNr];
}

char* Train::getName() {
  return this->name;
}

byte Train::getSpeedMsg() {
  byte  data;
  byte locoSpeed;
  bool dir;
  if (this->speed >0) {
    dir = true;
  } else {
    dir = false;
  }
  locoSpeed = abs(this->speed);
  if (locoSpeed == 1)  {  // this would result in emergency stop
    locoSpeed = 0;
  }
   
  // direction info first
  if (dir)  {  // forward
    data = 0b10000000;
  }  else  {
    data = 0;
  }
   
  data |=  locoSpeed;
  return data;
}
byte Train::getID() {
  return this->id;
}

bool Train::isConfigured() {
  return this->configured;
}

byte Train::getFunctionGroup1() {

  //Function Group One Instruction (100)
  //The format of this instruction is 100DDDDD

/*Up to 5 auxiliary functions (functions FL and F1-F4) can be controlled by the Function Group One instruction. Bits
0-3 shall define the value of functions F1-F4 with function F1 being controlled by bit 0 and function F4 being
controlled by bit 3. A value of "1" shall indicate that the function is "on" while a value of "0" shall indicate that the
function is "off". If Bit 1 of CV#29 has a value of one (1), then bit 4 controls function FL, otherwise bit 4 has no meaning. */

  byte data = 0b10000000;

  if (func[0]) {
    bitSet(data,4); 
  }
  if (func[1]) {
    bitSet(data,0); 
  }
  if (func[2]) {
    bitSet(data,1); 
  }
  if (func[3]) {
    bitSet(data,2); 
  }
  if (func[4]) {
    bitSet(data,3); 
  }
  return data;
}
byte Train::getFunctionGroup2() {

  //Function Group Two Instruction (101)
  //The format of this instruction is 101SDDDD

/*
Up to 8 additional auxiliary functions (F5-F12) can be controlled by a Function Group Two instruction. Bit 4
defines the use of Bits 0-3. When bit 4 (S) is ‘1’, Bits 0-3 (DDDD) shall define the value of functions F5-F8 with
 function F5 being controlled by bit 0 and function F8 being controlled by bit 3. When bit 4 (S) is ‘0’, Bits 0-3
(DDDD) shall define the value of functions F9-F12 with function F9 being controlled by bit 0 and function F12
being controlled by bit 3. A value of "1" shall indicate that the function is "on" while a value of "0" shall indicate
that the function is "off".  */

  byte data = 0b10100000;

  if (func[5]) {
    bitSet(data,0); 
  }
  if (func[6]) {
    bitSet(data,1); 
  }
  if (func[7]) {
    bitSet(data,2); 
  }
  if (func[8]) {
    bitSet(data,3); 
  }
  return data;
}
byte Train::getFunctionGroup3() {

  //Function Group Two Instruction (101)
  //The format of this instruction is 101SDDDD

/*
Up to 8 additional auxiliary functions (F5-F12) can be controlled by a Function Group Two instruction. Bit 4
defines the use of Bits 0-3. When bit 4 (S) is ‘1’, Bits 0-3 (DDDD) shall define the value of functions F5-F8 with
 function F5 being controlled by bit 0 and function F8 being controlled by bit 3. When bit 4 (S) is ‘0’, Bits 0-3
(DDDD) shall define the value of functions F9-F12 with function F9 being controlled by bit 0 and function F12
being controlled by bit 3. A value of "1" shall indicate that the function is "on" while a value of "0" shall indicate
that the function is "off".  */

  byte data = 0b10110000;

  if (func[9]) {
    bitSet(data,0); 
  }
  if (func[10]) {
    bitSet(data,1); 
  }
  if (func[11]) {
    bitSet(data,2); 
  }
  if (func[12]) {
    bitSet(data,3); 
  }
  return data;
}

bool Train::getFunction(int funcNr) {
  if (func[funcNr]) {
    return true;
  }
  return false;
}


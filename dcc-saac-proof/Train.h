
class Train {
  private:
    int speed;
    byte id;
    bool light;
    bool func[20];
    char name[9];
    bool configured; // true if the train is configured and usable
 
  public:
    Train(int idIn, String nameIn);
    void setSpeed(int inSpeed);
    int getSpeed();
    void setLight(bool inLight);
    bool getLight();
    void toggleFunction(int funcNr);
    char* Train::getName();

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

bool Train::getLight() {
  bool out;
  out = light;
  return out;
}

void Train::setLight(bool inLight) {
  light = inLight;
}

void Train::toggleFunction(int funcNr) {
  func[funcNr] = !func[funcNr];
}

char* Train::getName() {
  return this->name;
}


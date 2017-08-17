  int pin12 = 12;
  int pin11 = 11;
  int curDegrees = 0;
  bool trigger = false;
  bool triggered = false;
  
  void setup() {
    Serial.begin(9600);
    pinMode(pin12, INPUT);
    pinMode(pin11, INPUT);
  }
  
  void loop() {
    if (digitalRead(pin12) == LOW) {
      if (!triggered) {
        triggered = true;
        trigger = true; 
      }
      else {
        trigger = false;
      }
    }
    else {
      triggered = false;
    }
    
    if (trigger) {
      if (curDegrees < 300) {
          curDegrees += 60;
      }
      else {
        curDegrees = 0;
      } 
    }
    Serial.println(curDegrees);
    
    if (digitalRead(pin11) == LOW) {
      Serial.println("11");
    }
  }

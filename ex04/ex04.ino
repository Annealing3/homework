bool ledState = false;
unsigned long LTime;
int i=0;

void gotTouch() {
  ledState = !ledState;
  digitalWrite(2, ledState);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(2, OUTPUT);
}

void loop() {
  if(touchRead(4)<300){
    LTime=millis();
    while(1){
      if(touchRead(4)<300){
        if((millis()-LTime>50)&&i==1){
          gotTouch();
          i=0;
          break;
        }
      }
      else break;
    }
  }
  else i=1;
}
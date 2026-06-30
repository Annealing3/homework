bool ledState = false;
unsigned long LTime;
int Speed=15;
int i=0;
int j=0;
int pwmValue;
float ratio;

void Touch(){
  if(touchRead(4)<300){
    LTime=millis();
    while(1){
      if(touchRead(4)<300){
        if((millis()-LTime>50)&&j==1){
          j=0;
          if(Speed!=1) Speed-=7;
          else Speed=15;
          break;
        }
      }
      else break;
    }
  }
  else j=1;
}

void setup(){
  Serial.begin(115200);
  delay(1000);
  ledcAttach(2,5000,8);
}

void loop(){
  for(i=0;i<=255;i++){
    ratio=(float)i/255.0;
    pwmValue=(int)(pow(ratio,2.8)*255.0);
    ledcWrite(2,pwmValue);
    Touch();
    delay(Speed);
  }
  for(i=255;i>=0;i--){
    ratio=(float)i/255.0;
    pwmValue=(int)(pow(ratio,2.8)*255.0);
    ledcWrite(2,pwmValue);
    Touch();
    delay(Speed);
  }
}
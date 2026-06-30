int i=0;
int pwmValue;
float ratio;

void setup(){
  Serial.begin(115200);
  delay(1000);
  ledcAttach(4,5000,8);
  ledcAttach(5,5000,8);
}

void loop(){
  for(i=0;i<=255;i++){
    ratio=(float)i/255.0;
    pwmValue=(int)(pow(ratio,2.8)*255.0);
    ledcWrite(4,pwmValue);
    pwmValue=255-pwmValue;
    ledcWrite(5,pwmValue);
    delay(10);
  }
  for(i=255;i>=0;i--){
    ratio=(float)i/255.0;
    pwmValue=(int)(pow(ratio,2.8)*255.0);
    ledcWrite(4,pwmValue);
    pwmValue=255-pwmValue;
    ledcWrite(5,pwmValue);
    delay(10);
  }
}
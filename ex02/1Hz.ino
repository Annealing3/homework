// ex02/lab02.ino
// 使用 millis() 实现 1Hz 闪烁（亮500ms，灭500ms），无阻塞

const int ledPin = 2;          // ESP32 板载 LED 通常为 GPIO2（根据你的板子调整）

unsigned long previousMillis = 0;
const long interval = 500;     // 切换间隔 500ms
bool ledState = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // 非阻塞计时：如果距离上次切换超过 500ms，则翻转状态
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;   // 记录本次切换时刻

    ledState = !ledState;             // 翻转状态
    digitalWrite(ledPin, ledState);
  }
}
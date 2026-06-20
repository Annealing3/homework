// ex03/lab03.ino
// 使用 millis() 发送 SOS 信号（非阻塞）
// SOS = 3短闪 + 3长闪 + 3短闪，结束后长熄灭1.5秒

const int ledPin = 2;

// ----- SOS 参数定义（单位：毫秒）-----
const int totalSteps = 9;  // 总共9次闪动

// 每次闪动的“亮灯”持续时间
const int onDuration[totalSteps] = {
  200, 200, 200,   // 3次短闪（S）
  600, 600, 600,   // 3次长闪（O）
  200, 200, 200    // 3次短闪（S）
};

// 每次闪动后的“熄灯”持续时间
// 注意：第9次（索引8）结束后是长暂停1500ms，然后从头开始
const int offDuration[totalSteps] = {
  200, 200, 400,   // 第3次短闪后，停400ms（字母间隔）
  200, 200, 400,   // 第6次长闪后，停400ms（字母间隔）
  200, 200, 1500   // 第9次短闪后，停1500ms（单词间隔，结束后重置）
};

// ----- 状态变量 -----
int currentStep = 0;            // 当前执行到第几步（0~8）
bool isLedOn = false;           // 当前LED是否亮着
unsigned long stepStartTime = 0; // 当前步骤开始的时间戳

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);    // 初始熄灭
  stepStartTime = millis();     // 记录起始时间
}

void loop() {
  unsigned long currentMillis = millis();
  unsigned long elapsed = currentMillis - stepStartTime;

  // ---------- 当前 LED 是熄灭状态 ----------
  if (!isLedOn) {
    // 等待“熄灯持续时间”结束，然后点亮
    if (elapsed >= offDuration[currentStep]) {
      
      // 检查是否完成了整个 SOS 周期（即第9步熄灯结束）
      if (currentStep == totalSteps - 1) {
        currentStep = 0;        // 重置到第0步，重新开始SOS
      } else {
        currentStep++;          // 进入下一步
      }

      // 点亮 LED，标记为亮灯状态，重置计时器
      digitalWrite(ledPin, HIGH);
      isLedOn = true;
      stepStartTime = currentMillis;
    }
  }
  // ---------- 当前 LED 是点亮状态 ----------
  else {
    // 等待“亮灯持续时间”结束，然后熄灭
    if (elapsed >= onDuration[currentStep]) {
      
      // 熄灭 LED，标记为熄灯状态，重置计时器
      digitalWrite(ledPin, LOW);
      isLedOn = false;
      stepStartTime = currentMillis;
    }
  }
}
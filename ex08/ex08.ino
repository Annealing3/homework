#include <WiFi.h>
const char* ssid = "Redmi K70";
const char* password = "gt123456";
WiFiServer server(80);
int i = 0;
int j = 0;

String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>安防报警器</title>
    <style>
        body { font-family: Arial; text-align: center; margin-top: 50px; }
        button { font-size: 24px; padding: 15px 30px; margin: 10px; }
        .arm { background-color: #4CAF50; color: white; }
        .disarm { background-color: #f44336; color: white; }
    </style>
</head>
<body>
    <h1>安防报警器</h1>
    <p>点击按钮发送指令：</p>
    <button class="arm" onclick="sendCmd(1)">布防 (1)</button>
    <button class="disarm" onclick="sendCmd(0)">撤防 (0)</button>

    <script>
        function sendCmd(value) {
            // 发送 GET 请求到 /set?value=0 或 /set?value=1
            fetch('/set?value=' + value)
                .then(response => {
                    if (response.ok) {
                        console.log('指令发送成功：' + value);
                    } else {
                        console.error('发送失败');
                    }
                })
                .catch(error => console.error('网络错误:', error));
        }
    </script>
</body>
</html>
)rawliteral";

void Alarm(){
  digitalWrite(2, HIGH);
  delay(100);
  digitalWrite(2, LOW);
  delay(100);
}

void gotTouch() {
  if(i) j=1;
}

void news() {
    WiFiClient client = server.available();
    if (!client) {
        return;
    }
    String request = client.readStringUntil('\r');
    client.flush();
    if (request.indexOf("GET /set?value=") != -1) {
        int pos = request.indexOf("=");
        char ch = request.charAt(pos + 1);
        i = ch - '0';
        if(!i) j=0;
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println();
        client.println("OK");
    } else {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();
        client.println(htmlPage);
    }
    client.stop();
}

void setup(){
  Serial.begin(115200);
  delay(1000);
  pinMode(2, OUTPUT);
  touchAttachInterrupt(4,gotTouch,300);
  WiFi.begin(ssid, password);
  while (WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi 已连接");
  Serial.print("IP 地址: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  news();
  if(i&&j) Alarm();
}
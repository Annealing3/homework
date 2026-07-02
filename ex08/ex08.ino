#include <WiFi.h>

// ---------- AP 配置（替换掉原来的 ssid/password） ----------
const char* ap_ssid = "ESP32_Alarm";      // 热点名称
const char* ap_password = "12345678";     // 密码至少8位

WiFiServer server(80);
int i = 0;   // 0=撤防, 1=布防
int j = 0;   // 0=未触发, 1=触发报警

// ---------- 修改后的网页（增加状态显示区域和定时刷新） ----------
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
        .status { font-size: 28px; font-weight: bold; margin: 20px; }
    </style>
</head>
<body>
    <h1>安防报警器</h1>
    <p>当前状态：<span class="status" id="statusText">获取中...</span></p>
    <p>点击按钮发送指令：</p>
    <button class="arm" onclick="sendCmd(1)">布防 (1)</button>
    <button class="disarm" onclick="sendCmd(0)">撤防 (0)</button>

    <script>
        function sendCmd(value) {
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

        // 定时获取状态并更新页面
        function fetchStatus() {
            fetch('/status')
                .then(response => response.text())
                .then(data => {
                    document.getElementById('statusText').textContent = data;
                })
                .catch(error => console.error('状态获取失败:', error));
        }

        // 每500毫秒刷新一次状态
        setInterval(fetchStatus, 500);
        // 页面加载后立即获取一次
        fetchStatus();
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

    // ---------- 处理 /set 请求 ----------
    if (request.indexOf("GET /set?value=") != -1) {
        int pos = request.indexOf("=");
        char ch = request.charAt(pos + 1);
        i = ch - '0';
        if(!i) j=0;   // 撤防时清除报警状态
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println();
        client.println("OK");
    }
    // ---------- 处理 /status 请求（新增） ----------
    else if (request.indexOf("GET /status") != -1) {
        String status;
        if (i == 0) {
            status = "已撤防";
        } else if (i == 1 && j == 0) {
            status = "已布防";
        } else if (i == 1 && j == 1) {
            status = "已触发";
        } else {
            status = "未知";
        }
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println();
        client.println(status);
    }
    // ---------- 处理根请求，返回网页 ----------
    else {
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
  touchAttachInterrupt(4, gotTouch, 300);

  // ---------- 改为 AP 模式 ----------
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("AP 模式已启动");
  Serial.print("热点名称: ");
  Serial.println(ap_ssid);
  Serial.print("IP 地址: ");
  Serial.println(WiFi.softAPIP());   // 固定 192.168.4.1

  server.begin();
}

void loop(){
  news();
  if(i && j) Alarm();
}
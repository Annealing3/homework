#include <WiFi.h>
const char* ssid = "Redmi K70";
const char* password = "gt123456";
WiFiServer server(80);
int currentBrightness = 0;


String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 无极调光器</title>
    <style>
        body { font-family: Arial; text-align: center; margin-top: 50px; }
        .slider { width: 80%; max-width: 400px; }
        .value { font-size: 24px; font-weight: bold; }
    </style>
</head>
<body>
    <h1>ESP32 调光器</h1>
    <p>拖动滑块改变 LED 亮度</p>
    <input type="range" min="0" max="255" value="0" class="slider" id="brightnessSlider" oninput="updateBrightness(this.value)">
    <p>亮度值: <span class="value" id="brightnessValue">0</span></p>
    <script>
        function updateBrightness(value) {
            document.getElementById('brightnessValue').textContent = value;
            // 发送 GET 请求到 /update?value=xxx
            fetch('/update?value=' + value)
                .then(response => {
                    if (!response.ok) {
                        console.error('请求失败');
                    }
                })
                .catch(error => console.error('错误:', error));
        }
    </script>
</body>
</html>
)rawliteral";

void handleClient(WiFiClient client) {
    String request = client.readStringUntil('\r');
    client.flush();
    if (request.indexOf("GET /update?value=") != -1) {
        int start = request.indexOf("=") + 1;
        int end = request.indexOf(" ", start);
        String valueStr = request.substring(start, end);
        int value = valueStr.toInt();
        if (value >= 0 && value <= 255) {
            currentBrightness = value;
            ledcWrite(2, currentBrightness);
            Serial.print("亮度设置为: ");
            Serial.println(currentBrightness);
        }
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println("Connection: close");
        client.println();
        client.println("OK");
    } else {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println(htmlPage);
    }
    delay(1);
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    ledcAttach(2,5000,8);
    Serial.print("正在连接 WiFi");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi 已连接");
    Serial.print("IP 地址: ");
    Serial.println(WiFi.localIP());
    server.begin();
    Serial.println("HTTP 服务器已启动");
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        Serial.println("新客户端连接");
        handleClient(client);
        client.stop();
        Serial.println("客户端断开");
    }
}
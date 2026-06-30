#include <WiFi.h>

const char* ssid = "Redmi K70";
const char* password = "gt123456";
WiFiServer server(80);

String htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>触摸传感器仪表盘</title>
    <style>
        body { text-align: center; font-family: Arial; margin-top: 80px; }
        .value { font-size: 80px; font-weight: bold; color: #007BFF; }
        .unit { font-size: 30px; }
    </style>
</head>
<body>
    <h1>实时触摸值</h1>
    <div class="value" id="sensorValue">--</div>
    <div class="unit">（触摸时数值变小）</div>

    <script>
        function fetchData() {
            fetch('/data')
                .then(response => response.text())   // 获取纯文本
                .then(data => {
                    document.getElementById('sensorValue').textContent = data;
                })
                .catch(error => console.error('Error:', error));
        }

        // 每 200 毫秒拉取一次数据
        setInterval(fetchData, 200);

        // 页面加载后立即获取一次
        fetchData();
    </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();
    if (request.indexOf("GET /data") != -1) {
      int val = touchRead(4);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.println(val);
    } else {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      client.println(htmlPage);
    }
    client.stop();
  }
}
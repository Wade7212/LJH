#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "LJH";
const char* password = "qrewwc7212";

const int TOUCH_PIN = 32;

WebServer server(80);

String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ex09 实时传感器Web仪表盘</title>
  <style>
    body {
      font-family: Arial;
      text-align: center;
      margin-top: 50px;
      background-color: #f5f5f5;
    }
    .dashboard {
      width: 90%;
      max-width: 420px;
      margin: auto;
      padding: 30px;
      background-color: white;
      border-radius: 12px;
      box-shadow: 0 0 10px rgba(0,0,0,0.15);
    }
    .value {
      font-size: 64px;
      color: #0078d7;
      margin: 30px 0;
      font-weight: bold;
    }
    .label {
      font-size: 20px;
      color: #555;
    }
  </style>
</head>
<body>
  <div class="dashboard">
    <h1>实时传感器Web仪表盘</h1>
    <div class="label">触摸传感器实时数值</div>
    <div class="value" id="touchValue">--</div>
    <p>手指逐渐靠近触摸引脚时，数值会变小；手指离开后，数值会恢复。</p>
  </div>

  <script>
    function updateTouchValue() {
      fetch("/touch")
        .then(response => response.text())
        .then(data => {
          document.getElementById("touchValue").innerText = data;
        })
        .catch(error => {
          document.getElementById("touchValue").innerText = "Error";
          console.log(error);
        });
    }

    setInterval(updateTouchValue, 300);
    updateTouchValue();
  </script>
</body>
</html>
)rawliteral";

  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

void handleTouch() {
  int touchValue = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(touchValue));
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("ex09 Web sensor dashboard started");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("Visit: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/touch", handleTouch);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
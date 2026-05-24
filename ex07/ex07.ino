#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ljhesp";
const char* password = "qrewwc7212";

const int LED_PIN = 2;

WebServer server(80);

int brightness = 0;

String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ex07 Web无极调光器</title>
  <style>
    body {
      font-family: Arial;
      text-align: center;
      margin-top: 50px;
      background-color: #f5f5f5;
    }
    .card {
      width: 90%;
      max-width: 420px;
      margin: auto;
      padding: 25px;
      background-color: white;
      border-radius: 12px;
      box-shadow: 0 0 10px rgba(0,0,0,0.15);
    }
    input[type=range] {
      width: 90%;
      margin-top: 25px;
    }
    .value {
      font-size: 36px;
      color: #0078d7;
      margin-top: 20px;
    }
  </style>
</head>
<body>
  <div class="card">
    <h1>Web网页端无极调光器</h1>
    <p>拖动滑动条，实时控制LED亮度</p>

    <input 
      type="range" 
      id="brightnessSlider" 
      min="0" 
      max="255" 
      value="0"
    >

    <div class="value">
      当前亮度：<span id="brightnessValue">0</span>
    </div>
  </div>

  <script>
    const slider = document.getElementById("brightnessSlider");
    const valueText = document.getElementById("brightnessValue");

    slider.addEventListener("input", function() {
      const value = slider.value;
      valueText.innerText = value;

      fetch("/setBrightness?value=" + value)
        .then(response => response.text())
        .then(data => {
          console.log(data);
        })
        .catch(error => {
          console.log("Request failed", error);
        });
    });
  </script>
</body>
</html>
)rawliteral";

  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

void handleSetBrightness() {
  if (server.hasArg("value")) {
    brightness = server.arg("value").toInt();

    if (brightness < 0) {
      brightness = 0;
    }

    if (brightness > 255) {
      brightness = 255;
    }

    analogWrite(LED_PIN, brightness);

    Serial.print("Brightness: ");
    Serial.println(brightness);

    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing value");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, brightness);

  Serial.println();
  Serial.println("ex07 Web dimmer started");

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
  server.on("/setBrightness", handleSetBrightness);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
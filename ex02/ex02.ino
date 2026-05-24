#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ljhesp";
const char* password = "qrewwc7212";

const int LED_PIN = 2;   // GPIO2，常见板载 LED

WebServer server(80);

int ledBrightness = 0;   // LED亮度，范围 0~255

String makePage() {
  String state = ledBrightness > 0 ? "ON" : "OFF";

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>实验2 - LED亮度控制</title>
</head>
<body style="font-family:Arial; text-align:center; margin-top:50px;">
  <h1>第二部分：按钮与滑动条控制 LED</h1>

  <p>当前状态：<b>)rawliteral" + state + R"rawliteral(</b></p>
  <p>当前亮度：<b>)rawliteral" + String(ledBrightness) + R"rawliteral(</b></p>

  <a href="/on">
    <button style="padding:10px 20px; margin:10px;">点亮 LED</button>
  </a>

  <a href="/off">
    <button style="padding:10px 20px; margin:10px;">熄灭 LED</button>
  </a>

  <h2>亮度调节</h2>

  <input 
    type="range" 
    min="0" 
    max="255" 
    value=")rawliteral" + String(ledBrightness) + R"rawliteral(" 
    oninput="brightnessValue.innerText=this.value"
    onchange="location.href='/brightness?value=' + this.value"
    style="width:300px;"
  >

  <p>滑动条数值：<span id="brightnessValue">)rawliteral" + String(ledBrightness) + R"rawliteral(</span></p>

  <p>说明：0 表示熄灭，255 表示最亮。</p>
</body>
</html>
)rawliteral";

  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

void handleOn() {
  ledBrightness = 255;
  analogWrite(LED_PIN, ledBrightness);

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff() {
  ledBrightness = 0;
  analogWrite(LED_PIN, ledBrightness);

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleBrightness() {
  if (server.hasArg("value")) {
    ledBrightness = server.arg("value").toInt();

    if (ledBrightness < 0) {
      ledBrightness = 0;
    }

    if (ledBrightness > 255) {
      ledBrightness = 255;
    }

    analogWrite(LED_PIN, ledBrightness);
  }

  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("ESP32 program started");

  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, ledBrightness);

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
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/brightness", handleBrightness);

  server.begin();

  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
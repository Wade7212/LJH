#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ljhesp";
const char* password = "qrewwc7212";

const int LED_PIN = 2;
const int TOUCH_PIN = T0;

const int TOUCH_THRESHOLD = 30;

WebServer server(80);

bool armed = false;
bool alarmTriggered = false;

unsigned long previousBlinkMillis = 0;
const unsigned long BLINK_INTERVAL = 80;
bool ledState = false;

bool isTouched() {
  int touchValue = touchRead(TOUCH_PIN);

  if (touchValue < TOUCH_THRESHOLD) {
    return true;
  } else {
    return false;
  }
}

String makePage() {
  String armState = armed ? "已布防 Arm" : "未布防 Disarm";
  String alarmState = alarmTriggered ? "报警中 Alarm" : "正常 Normal";

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ex08 物联网安防报警器</title>
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
    button {
      padding: 12px 25px;
      margin: 10px;
      font-size: 18px;
      border: none;
      border-radius: 8px;
    }
    .arm {
      background-color: #e74c3c;
      color: white;
    }
    .disarm {
      background-color: #2ecc71;
      color: white;
    }
    .status {
      font-size: 22px;
      margin: 20px;
    }
  </style>
</head>
<body>
  <div class="card">
    <h1>物联网安防报警器</h1>

    <div class="status">
      系统状态：<b>)rawliteral" + armState + R"rawliteral(</b>
    </div>

    <div class="status">
      报警状态：<b>)rawliteral" + alarmState + R"rawliteral(</b>
    </div>

    <a href="/arm">
      <button class="arm">布防 Arm</button>
    </a>

    <a href="/disarm">
      <button class="disarm">撤防 Disarm</button>
    </a>

    <p>说明：布防后触碰触摸引脚，LED将高频闪烁并锁定报警。</p>
  </div>
</body>
</html>
)rawliteral";

  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

void handleArm() {
  armed = true;
  alarmTriggered = false;
  ledState = false;
  digitalWrite(LED_PIN, LOW);

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleDisarm() {
  armed = false;
  alarmTriggered = false;
  ledState = false;
  digitalWrite(LED_PIN, LOW);

  server.sendHeader("Location", "/");
  server.send(303);
}

void updateAlarmBlink() {
  if (alarmTriggered) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousBlinkMillis >= BLINK_INTERVAL) {
      previousBlinkMillis = currentMillis;

      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    }
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println();
  Serial.println("ex08 IoT security alarm started");

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
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();

  if (armed && !alarmTriggered) {
    if (isTouched()) {
      alarmTriggered = true;
      Serial.println("Alarm triggered!");
    }
  }

  updateAlarmBlink();
}
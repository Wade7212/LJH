const int LED_PIN = 2;
const int TOUCH_PIN = T0;

const int TOUCH_THRESHOLD = 300;
const unsigned long DEBOUNCE_TIME = 200;

bool lastTouchState = false;
unsigned long lastTriggerTime = 0;

int speedLevel = 1;

int brightness = 0;
int fadeDirection = 1;

bool isTouched() {
  int touchValue = touchRead(TOUCH_PIN);

  if (touchValue < TOUCH_THRESHOLD) {
    return true;
  } else {
    return false;
  }
}

int getDelayTime() {
  if (speedLevel == 1) {
    return 20;
  } else if (speedLevel == 2) {
    return 10;
  } else {
    return 4;
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  bool currentTouchState = isTouched();
  unsigned long currentMillis = millis();

  if (lastTouchState == false && currentTouchState == true) {
    if (currentMillis - lastTriggerTime >= DEBOUNCE_TIME) {
      speedLevel++;

      if (speedLevel > 3) {
        speedLevel = 1;
      }

      Serial.print("Current speed level: ");
      Serial.println(speedLevel);

      lastTriggerTime = currentMillis;
    }
  }

  lastTouchState = currentTouchState;

  analogWrite(LED_PIN, brightness);

  brightness = brightness + fadeDirection * 5;

  if (brightness >= 255) {
    brightness = 255;
    fadeDirection = -1;
  }

  if (brightness <= 0) {
    brightness = 0;
    fadeDirection = 1;
  }

  delay(getDelayTime());
}
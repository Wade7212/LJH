const int LED_PIN = 32;
const int TOUCH_PIN = T0;

const int TOUCH_THRESHOLD = 300;
const unsigned long DEBOUNCE_TIME = 200;

bool ledState = false;
bool lastTouchState = false;

unsigned long lastTriggerTime = 0;

bool isTouched() {
  int touchValue = touchRead(TOUCH_PIN);

  if (touchValue < TOUCH_THRESHOLD) {
    return true;
  } else {
    return false;
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
}

void loop() {
  bool currentTouchState = isTouched();
  unsigned long currentMillis = millis();

  if (lastTouchState == false && currentTouchState == true) {
    if (currentMillis - lastTriggerTime >= DEBOUNCE_TIME) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);

      lastTriggerTime = currentMillis;
    }
  }

  lastTouchState = currentTouchState;

  Serial.println(touchRead(TOUCH_PIN));
}
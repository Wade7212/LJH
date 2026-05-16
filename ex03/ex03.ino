const int LED_PIN = 23;

const unsigned long DOT_TIME = 200;
const unsigned long DASH_TIME = 600;
const unsigned long GAP_TIME = 200;
const unsigned long END_GAP_TIME = 1200;

enum LedState {
  LED_ON,
  LED_OFF
};

struct SignalStep {
  LedState state;
  unsigned long duration;
};

SignalStep sosPattern[] = {
  {LED_ON, DOT_TIME}, {LED_OFF, GAP_TIME},
  {LED_ON, DOT_TIME}, {LED_OFF, GAP_TIME},
  {LED_ON, DOT_TIME}, {LED_OFF, GAP_TIME},

  {LED_ON, DASH_TIME}, {LED_OFF, GAP_TIME},
  {LED_ON, DASH_TIME}, {LED_OFF, GAP_TIME},
  {LED_ON, DASH_TIME}, {LED_OFF, GAP_TIME},

  {LED_ON, DOT_TIME}, {LED_OFF, GAP_TIME},
  {LED_ON, DOT_TIME}, {LED_OFF, GAP_TIME},
  {LED_ON, DOT_TIME}, {LED_OFF, END_GAP_TIME}
};

const int stepCount = sizeof(sosPattern) / sizeof(sosPattern[0]);

int currentStep = 0;
unsigned long previousMillis = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= sosPattern[currentStep].duration) {
    previousMillis = currentMillis;

    currentStep++;

    if (currentStep >= stepCount) {
      currentStep = 0;
    }

    if (sosPattern[currentStep].state == LED_ON) {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }
  }
}
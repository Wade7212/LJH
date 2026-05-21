const int LED_A_PIN = 16;
const int LED_B_PIN = 17;

int brightness = 0;
int fadeDirection = 1;

const int STEP = 5;
const int DELAY_TIME = 20;

void setup() {
  pinMode(LED_A_PIN, OUTPUT);
  pinMode(LED_B_PIN, OUTPUT);
}

void loop() {
  int brightnessA = brightness;
  int brightnessB = 255 - brightness;

  analogWrite(LED_A_PIN, brightnessA);
  analogWrite(LED_B_PIN, brightnessB);

  brightness = brightness + fadeDirection * STEP;

  if (brightness >= 255) {
    brightness = 255;
    fadeDirection = -1;
  }

  if (brightness <= 0) {
    brightness = 0;
    fadeDirection = 1;
  }

  delay(DELAY_TIME);
}
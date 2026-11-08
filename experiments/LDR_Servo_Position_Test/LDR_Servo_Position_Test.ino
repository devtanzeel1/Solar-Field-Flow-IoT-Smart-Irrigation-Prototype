#include <Servo.h>

// Standalone experiment based on the project's separate LDR/servo sketch.
// This is a single-LDR two-position test, not a dual-LDR tracking algorithm.

const uint8_t LDR_PIN = D6;
const uint8_t SERVO_PIN = D3;
const int LIGHT_DETECTED_STATE = LOW;
const int POSITION_WITH_LIGHT = 100;
const int POSITION_WITHOUT_LIGHT = 0;

Servo trackingServo;

void setup() {
  Serial.begin(9600);
  pinMode(LDR_PIN, INPUT);
  trackingServo.attach(SERVO_PIN);
  trackingServo.write(POSITION_WITHOUT_LIGHT);
}

void loop() {
  const int ldrState = digitalRead(LDR_PIN);
  const int targetPosition = (ldrState == LIGHT_DETECTED_STATE)
                                 ? POSITION_WITH_LIGHT
                                 : POSITION_WITHOUT_LIGHT;

  trackingServo.write(targetPosition);

  Serial.print(F("LDR state: "));
  Serial.print(ldrState);
  Serial.print(F(" | Servo target: "));
  Serial.println(targetPosition);

  delay(500);
}

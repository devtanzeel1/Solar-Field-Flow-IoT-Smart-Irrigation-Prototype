#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleDHT.h>
#include "secrets.h"

// ---------- Hardware pins (ESP8266 NodeMCU-style labels) ----------
const uint8_t SOIL_MOISTURE_PIN = A0;
const uint8_t PUMP_RELAY_PIN    = D7;
const uint8_t BUZZER_PIN        = D4;
const uint8_t DHT11_PIN         = D0;
const uint8_t ALERT_INPUT_PIN   = D5;

// The source archive called D5 fireSensorPin. The supplied prototype
// photograph labels the module PIR. Confirm the physical sensor type and
// active polarity before relying on this input.

// ---------- Calibration and thresholds ----------
const int SOIL_DRY_READING = 700;
const int SOIL_WET_READING = 400;
const int LOW_MOISTURE_THRESHOLD = 20;
const byte HIGH_TEMPERATURE_THRESHOLD_C = 40;

// Original relay logic is active LOW: LOW = pump ON, HIGH = pump OFF.
const uint8_t RELAY_ON  = LOW;
const uint8_t RELAY_OFF = HIGH;

// ---------- Devices ----------
SimpleDHT11 dht11;
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

// ---------- Last sampled values ----------
int soilMoisturePercent = 0;
byte temperatureC = 0;
byte humidityPercent = 0;
int dhtStatus = -1;
int alertInputState = LOW;

int calculateSoilMoisturePercent(int rawReading) {
  // 0% at the configured dry reading; 100% at the configured wet reading.
  const long mapped = map(rawReading, SOIL_DRY_READING, SOIL_WET_READING, 0, 100);
  return constrain((int)mapped, 0, 100);
}

void updateOutputs() {
  const bool soilIsDry = soilMoisturePercent < LOW_MOISTURE_THRESHOLD;
  const bool dhtReadFailed = (dhtStatus != SimpleDHTErrSuccess);
  const bool temperatureIsHigh = !dhtReadFailed &&
                                 temperatureC >= HIGH_TEMPERATURE_THRESHOLD_C;
  const bool alertInputActive = (alertInputState == HIGH);

  // Preserve the original prototype's control rule: pump runs when soil is dry.
  digitalWrite(PUMP_RELAY_PIN, soilIsDry ? RELAY_ON : RELAY_OFF);

  // Preserve the original buzzer alert conditions.
  const bool alarmActive = soilIsDry || temperatureIsHigh ||
                           dhtReadFailed || alertInputActive;
  digitalWrite(BUZZER_PIN, alarmActive ? HIGH : LOW);
}

void sampleAndPublish() {
  const int rawSoilReading = analogRead(SOIL_MOISTURE_PIN);
  soilMoisturePercent = calculateSoilMoisturePercent(rawSoilReading);

  temperatureC = 0;
  humidityPercent = 0;
  dhtStatus = dht11.read(DHT11_PIN, &temperatureC, &humidityPercent, NULL);
  alertInputState = digitalRead(ALERT_INPUT_PIN);

  updateOutputs();

  Blynk.virtualWrite(V0, soilMoisturePercent);
  Blynk.virtualWrite(V4, alertInputState);

  // Do not publish the zero-initialised values when the DHT11 read fails.
  if (dhtStatus == SimpleDHTErrSuccess) {
    Blynk.virtualWrite(V1, (int)temperatureC);
    Blynk.virtualWrite(V2, (int)humidityPercent);
  } else {
    Serial.println(F("DHT11 read failed; temperature/humidity not published."));
  }

  Serial.print(F("Soil moisture: "));
  Serial.print(soilMoisturePercent);
  Serial.print(F("% | Alert input: "));
  Serial.print(alertInputState);
  if (dhtStatus == SimpleDHTErrSuccess) {
    Serial.print(F(" | Temperature: "));
    Serial.print((int)temperatureC);
    Serial.print(F(" C | Humidity: "));
    Serial.print((int)humidityPercent);
    Serial.println(F("%"));
  } else {
    Serial.println();
  }
}

void updateLCD() {
  char row0[17];
  char row1[17];

  // The source firmware treated HIGH on the D5 digital input as an alert.
  snprintf(row0, sizeof(row0), "SM:%3d%% A:%d", soilMoisturePercent,
           alertInputState == HIGH ? 1 : 0);

  if (dhtStatus == SimpleDHTErrSuccess) {
    snprintf(row1, sizeof(row1), "T:%2dC H:%2d%%",
             (int)temperatureC, (int)humidityPercent);
  } else {
    snprintf(row1, sizeof(row1), "T:--C H:--%%");
  }

  char padded0[17];
  char padded1[17];
  snprintf(padded0, sizeof(padded0), "%-16s", row0);
  snprintf(padded1, sizeof(padded1), "%-16s", row1);

  lcd.setCursor(0, 0);
  lcd.print(padded0);
  lcd.setCursor(0, 1);
  lcd.print(padded1);
}

void setup() {
  Serial.begin(9600);

  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(ALERT_INPUT_PIN, INPUT);

  // Set outputs to their inactive states before connecting to Wi-Fi/Blynk.
  digitalWrite(PUMP_RELAY_PIN, RELAY_OFF);
  digitalWrite(BUZZER_PIN, LOW);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Solar Field Flow"));
  lcd.setCursor(0, 1);
  lcd.print(F("Connecting..."));

  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);

  sampleAndPublish();
  updateLCD();

  timer.setInterval(10000L, sampleAndPublish);
  timer.setInterval(2000L, updateLCD);
}

void loop() {
  Blynk.run();
  timer.run();
}

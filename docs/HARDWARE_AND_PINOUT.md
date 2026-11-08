# Hardware and pinout notes

## Controller identification

The supplied Solar Field Flow prototype photograph labels the board **ESP8266**. The integrated Blynk sketch uses `BlynkSimpleEsp8266.h` and ESP8266-style pin labels. This repository therefore targets an ESP8266 development board. Do not select an ESP32 board profile for this sketch.

## Integrated firmware pin map

| Function | ESP8266 pin label | Notes |
|---|---|---|
| Soil-moisture analogue output | `A0` | Confirm the board's A0 input voltage limit and sensor divider. |
| Pump relay input | `D7` | Active LOW in the source firmware. |
| Buzzer control | `D4` | HIGH activates the buzzer in the source logic. |
| DHT11 data | `D0` | Uses `SimpleDHT`. |
| Digital alert input | `D5` | Original code calls it a fire sensor; the prototype photograph labels a PIR sensor. Verify the actual module. |
| I²C LCD SDA | `D2` / GPIO4 | ESP8266 Wire default on common NodeMCU-style boards; confirm board variant. |
| I²C LCD SCL | `D1` / GPIO5 | ESP8266 Wire default on common NodeMCU-style boards; confirm board variant. |

LCD address in the source: `0x27`, 16 columns × 2 rows.

## Separate LDR/servo experiment

| Function | ESP8266 pin label |
|---|---|
| LDR digital output | `D6` |
| Servo signal | `D3` |

This experiment is separate from the Blynk irrigation firmware. Confirm the servo supply, grounds, pin boot behaviour and LDR module's active polarity before connecting it.

## Hardware visible in supplied prototype image

The image labels an ESP8266, soil-moisture sensor, water pump ON/OFF, relay, 7.4 V battery pack, PIR motion sensor, DHT11 temperature/humidity sensor and 16×2 I²C LCD. The source firmware's D5 signal is named `fireSensorPin`, so the physical sensor identity is unresolved by the supplied material.

## Electrical safety

- Never drive a pump, motor or servo directly from an ESP8266 GPIO.
- Use an appropriately rated relay/driver and a correctly rated external supply for loads.
- Check the specific ESP8266 board's analogue input maximum; some boards include an input divider and some bare modules do not.
- Ensure appropriate common ground and logic-level compatibility.
- Do not expose the low-voltage prototype to water or connect mains loads without suitable isolation, enclosure and qualified supervision.

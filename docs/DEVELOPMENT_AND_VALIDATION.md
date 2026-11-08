# Development and validation notes

## Source selection

The integrated firmware was based on the supplied `PROJECT CODE/CODE BLYNK SFF.txt`, which is the archive's combined ESP8266 + Blynk + soil moisture + DHT11 + relay + buzzer + LCD sketch.

The original integrated sketch also declared an LDR input and attached a servo, but did not read the LDR or command the servo. Those unused declarations were not presented as implemented features in the integrated sketch. A separate, clearly labelled single-LDR/servo experiment was retained under `experiments/` based on the supplied `TANZEEL LAST/2ccc.txt` experiment.

## Changes made for this portfolio copy

- Removed embedded Blynk and Wi-Fi credentials; moved them to an ignored local `secrets.h` file.
- Kept the original ESP8266 target, Blynk virtual-pin mapping, soil calibration values, pump threshold, DHT11 threshold, relay active-LOW assumption and alert-input HIGH assumption.
- Added a startup state that commands the relay OFF and buzzer OFF before connecting.
- Avoided publishing zero-initialised temperature/humidity values when a DHT11 read fails.
- Updated the LCD from cached readings rather than taking a second set of sensor readings.
- Separated the LDR/servo experiment from the integrated irrigation/Blynk sketch.

## Known limitations / items to verify

- The source archive calls the D5 input a fire sensor, while the supplied prototype photograph labels the sensor PIR. Confirm the actual module and active polarity.
- Soil ADC calibration values (700 dry, 400 wet) are inherited from the source and require calibration for the actual sensor, soil and board.
- ESP8266 analogue input range depends on the exact board variant and input divider.
- The relay is assumed active LOW. Confirm the actual relay module before connecting a pump.
- The single-LDR/servo experiment uses two fixed servo positions; it is not a complete dual-LDR solar-tracking algorithm.
- The Blynk sketch has not been compiled against the exact library versions used by the original project or tested on the physical prototype in this session.

## Suggested validation sequence

1. Compile with the exact ESP8266 board profile and library versions.
2. Test sensor readings with pump power disconnected.
3. Verify soil-moisture calibration in dry and wet reference conditions.
4. Verify DHT11 error handling and temperature/humidity readings.
5. Verify D5 sensor identity and output polarity.
6. Verify relay ON/OFF polarity with a safe low-voltage test load.
7. Verify buzzer output and LCD layout.
8. Configure Blynk datastreams and confirm V0, V1, V2 and V4 updates.
9. Test the pump with a suitable isolated supply and safe enclosure.
10. Test the LDR/servo experiment separately from the integrated firmware.

# Blynk configuration

The integrated firmware publishes the following datastreams:

| Virtual pin | Datapoint | Type / unit |
|---|---|---|
| `V0` | Soil moisture | Integer, percent (estimated from calibrated ADC values) |
| `V1` | Temperature | Integer, °C; sent only after a successful DHT11 read |
| `V2` | Humidity | Integer, %RH; sent only after a successful DHT11 read |
| `V4` | Digital alert input | Integer, 0 or 1 |

The original `CODE BLYNK SFF.txt` did not publish a `V3` value. The integrated firmware keeps the source's V0, V1, V2 and V4 mapping.

## Credentials

Copy `firmware/Solar_Field_Flow/secrets.example.h` to `secrets.h` in the same directory and fill in:

- Blynk template ID
- Blynk template name
- Blynk device authentication token
- Wi-Fi SSID
- Wi-Fi password

Do not commit `secrets.h`. It is ignored by the repository's `.gitignore`.

The original archive contained real Blynk and Wi-Fi credentials. Treat those credentials as exposed and rotate them before reconnecting the prototype.

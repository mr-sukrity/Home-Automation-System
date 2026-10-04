# Circuit Notes

## Relay Module

Connect the relay module to the ESP32 as follows:

| Relay | ESP32 |
|---|---|
| IN1 | GPIO 26 |
| IN2 | GPIO 27 |
| VCC | Appropriate module supply |
| GND | GND |

The example assumes an **active-LOW relay module**.

If your relay is active HIGH, change:

```cpp
#define RELAY_ON  LOW
#define RELAY_OFF HIGH
```

to:

```cpp
#define RELAY_ON  HIGH
#define RELAY_OFF LOW
```

## PIR Sensor

```text
PIR VCC  -> ESP32 supply
PIR GND  -> GND
PIR OUT  -> GPIO 14
```

## DHT11

```text
DHT11 VCC  -> 3.3V
DHT11 GND  -> GND
DHT11 DATA -> GPIO 4
```

## LDR

Build a voltage divider using an LDR and 10kΩ resistor.

```text
3.3V
 |
LDR
 |
 +------ GPIO 34
 |
10kΩ
 |
GND
```

GPIO 34 is input-only and is suitable for analog input.

## Important Safety Note

For a prototype, connect relay outputs to LEDs or low-voltage loads.

Do not work with exposed household mains voltage unless you have the appropriate electrical knowledge, isolation, enclosure, protection and supervision.

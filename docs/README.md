# Wokwi AT24CXX EEPROM Chip Example

The AT24CXX Custom Chip simulates the Microchip ATMEL AT24C eeproms.

## Pin names

| Name         | Description                                            |
| ------------ | ------------------------------------------------------ |
| `A0-A2`      | Address pins                                           |
| `SCL, SDA`   | THe I2C pins                                           |
| GND          | Ground                                                 |
| VCC          | Supply voltage                                         |
| `WP`         | Write protect pin                                      |

## Usage

To use this chip in your project, include it as a dependency in your `diagram.json` file:

```json
  "dependencies": {
    "chip-at24cxx": "github:venusz/at24cxx-chip@1.0.0"
  }
```

Then, add the chip to your circuit by adding a `chip-at24cxx` item to the `parts` section of diagram.json:

```json
  "parts": {
    ...,
    { "type": "chip-at24cxx", "id": "chip1" }
  },
```

For a complete example, see [the EEPROM chip test project](https://wokwi.com/projects/396384978929228801).
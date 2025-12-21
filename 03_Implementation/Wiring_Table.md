## Component Wiring 

### RFID (RC522)

| Component Pin | ESP32 GPIO Pin |
|--------------|----------------|
| VCC          | 3.3V           |
| GND          | GND            |
| RST          | GPIO 4         |
| IRQ          | –              |
| MISO         | GPIO 19        |
| MOSI         | GPIO 23        |
| SCK          | GPIO 18        |
| SDA (SS)     | GPIO 5         |

### LCD Display (I2C)

| Component Pin | ESP32 GPIO Pin |
|--------------|----------------|
| VCC          | 5V             |
| GND          | GND            |
| SDA          | GPIO 21        |
| SCL          | GPIO 22        |

### Relay Module

| Component Pin | ESP32 GPIO Pin |
|--------------|----------------|
| Signal       | GPIO 17        |
| VCC          | 5V             |
| GND          | GND            |

### Keypad (3×4)

| Component Pin | ESP32 GPIO Pin |
|--------------|----------------|
| R1           | GPIO 14        |
| R2           | GPIO 27        |
| R3           | GPIO 26        |
| R4           | GPIO 25        |
| C1           | GPIO 33        |
| C2           | GPIO 32        |
| C3           | GPIO 13        |

### Buzzer

| Component Pin | ESP32 GPIO Pin |
|--------------|----------------|
| Signal       | –              |
| VCC          | –              |
| GND          | –              |

### Reed Sensor

| Component Pin | ESP32 GPIO Pin |
|--------------|----------------|
| Pin 1        | GND            |
| Pin 2        | GPIO 15        |





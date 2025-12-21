## Hardware Components Used in This Project

|          Component          |                                   Purpose                                   |
| --------------------------  | --------------------------------------------------------------------------- |
| Raspberry Pi 4              | Serves as the local IoT gateway and application server to host Node-RED for control logic, InfluxDB for data storage, and Grafana for data visualization |
| ESP32 Microcontroller | Acts as the primary controller at the perception layer to handle sensor input, authentication logic, actuator control, and communication with Raspberry Pi |
| 3x4 Membrane Keypad | For keypad-based PIN input authentication for user access verification|
| RC522 RFID Reader and Tag | For contactless RFID-based authentication by verifying registered UID|
| Mobile Phone | Allows phone-based remote unlocking through HTTP requests send from the iPhone Shortcuts application|
| Solenoid Door Lock | Serves as the main physical locking mechanism that secures or releases the door
| 5V Relay Module | Electrically isolates and controls the high-current solenoid lock using signals from the ESP32|
| Lithium Ion Battery and Battery Holder | To provide power supply for the solenoid lock |
|Buzzer | Produces audio alerts during repeated invalid authentication attempts after exceeding an amount of time, or door open escalation events|
|Breadboard| To facilitate circuit connections during implementation|
|I2C LCD Display | Displays lock status messages, user prompts, and authentication feedback |
|Magnetic Reed Sensor (Reed Switch) |Detects the door’s open or closed state for real-time monitoring and escalation logic| 

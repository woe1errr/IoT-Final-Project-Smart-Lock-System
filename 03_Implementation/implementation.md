# 3. Implementation Details

## 3.1 Hardware Implementation and Circuit Connections

The hardware implementation of the IoT Smart Lock System is centered around the ESP32 microcontroller, which connects to multiple sensing, authentication, alert, and actuation components. The prototyping, testing and debugging is done on a breadboard where all the components are assembled.

The **RC522 RFID reader** is connected to the ESP32 using the SPI communication interface to enable contactless RFID-based authentication. A **3×4 membrane keypad** is connected via digital GPIO pins to support PIN-based authentication. User prompts, system status messages, and authentication feedback are displayed on an **I2C LCD display**, which reduces GPIO usage by utilizing the I2C communication protocol.

A **magnetic reed switch** is installed on the door frame to detect whether the door is open or closed. The reed switch output is monitored continuously by the ESP32 to support real-time door state tracking and door open escalation logic. An **active buzzer** is connected to a digital output pin to give immediate audio that would alert the users in conditions regarding prolonged door escalation time and repeated failed attempts.  

The **solenoid door lock** serves as the primary physical locking mechanism. The solenoid runs on a greater current than the ESP32 can comfortably deliver, so a **5V relay module** has been used to provide electrical isolation and control of the solenoid.  **18650 battery and battery holder** are also used to provide adequate power to the solenoid lock.

For a clearer overview of the hardware components and their respective purposes, refer to the [Hardware Components Table](./Hardware_Components.md).

The figure below shows the circuit connection diagram of the prototype. Detailed wiring information can be found in the [Wiring Table](./Wiring_Table.md).


<img width="3000" height="2919" alt="Circuit_Connection" src= "https://github.com/woe1errr/IoT-Final-Project-Smart-Lock-System/blob/main/Diagram/Circuit_Connection_Diagram.png">
Diagram 1: Circuit Connection Diagram

---

## 3.2 Firmware Design and Core Modules

The firmware for the ESP32 is developed using the Arduino IDE and written in C++. For better clarity, the code is divided into several modules. Besides, all authentication checks are performed locally on the ESP32 before any physical actuation occurs, ensuring that unauthorized inputs do not trigger the relay to activate the solenoid lock.

The core firmware modules include:

- **Authentication Module:**  
  This module handles RFID tag verification and keypad-based PIN validation. Only the tag with the correct predefined UID and authenticated inputs are allowed to move on with the lock unlock logic.

- **Sensor Monitoring Module:**
  The magnetic reed switch constantly monitors the door's open or closed states and generates corresponding status updates

- **Actuation Module:**  
  Controls the relay module to unlock or relock the solenoid door lock based on the authentication results and time escalation logic.

- **Buzzer Control Module:**  
  To generate audible alerts for repeated failed PIN attempts, and door open escalation events.

- **Communication Module:**  
  Manages Wi-Fi connectivity, MQTT client initialization, message publishing for door status and access events, and subscription to control topics for authorized remote commands.

---

## 3.3 Software Implementation

**Mosquitto MQTT Broker, Node-RED, iPhone Shortcuts, InfluxDB, Grafana**

The software implementation is hosted on a Raspberry Pi, which functions as the local IoT gateway and application server. The Raspberry Pi runs Raspberry Pi OS and hosts all networking, data processing, and visualization services.

A **Mosquitto MQTT broker** is deployed to manage publish/subscribe communication between the ESP32 and application-layer services. Username and password authentication are configured at the broker level to restrict unauthorized device connections.

**Node-RED** is used as the primary application logic platform. It subscribes to MQTT topics published by the ESP32 to process door status updates and authentication events. Node-RED also implements control logic, logging workflows, and an HTTP interface for mobile phone–based remote unlocking.

To support remote control, an HTTP endpoint is implemented in Node-RED. The mobile interface is provided through the **iPhone Shortcuts application**, which sends HTTP requests containing an API key in the request header. Each request is validated before any control command is forwarded to the ESP32 via MQTT.

For data management, **InfluxDB** is used as a local time-series database to store door state changes, alert status, and access activity logs. **Grafana** is integrated with InfluxDB to provide real-time and historical visualization of system activity through a web-based dashboard with different panels.

Lastly to simplify deployment and improve service isolation, the MQTT broker, Node-RED, InfluxDB, and Grafana are deployed using **Docker** containers on the Raspberry Pi. Docker Compose is used to manage and orchestrate these services, allowing consistent configuration, simplified startup, and ease of maintenance. Containerization ensures that each service runs in an isolated environment while communicating over the local network.

For a clearer overview of the software components and their respective purposes, refer to the [Software Components Table](./Software_Components.md)

---

## 3.4 Prototype and Test Environment

The system prototype is implemented using breadboard and jumper wires to allow flexibility during development, testing, and troubleshooting. The ESP32 is physically connected to the authentication components, sensors and actuator, while the Raspberry Pi serves as the local IoT gateway.

All application-layer services, including the MQTT broker, Node-RED, InfluxDB, and Grafana, are deployed on the Raspberry Pi using Docker containers managed through Docker Compose. This containerized deployment simplifies service setup, ensures consistent runtime environments, and allows individual services to be started, stopped, or reconfigured independently.

Testing is conducted within a controlled local network environment, where the ESP32, Raspberry Pi, and mobile phone are connected to the same Wi-Fi network. The test environment supports validation of authentication workflows, relay actuation, buzzer alerts, MQTT message exchange, HTTP-based remote control, and real-time data visualization.

Photographs of the physical prototype and wiring configuration, along with screenshots of the Docker containers in operation, Node-RED flows, MQTT message logs, database entries, and Grafana dashboards, are captured to provide evidence of correct system integration and functionality.

---

[Section 4: Networking Protocol](/04_Networking_Protocol/Protocol_Design_And_Data_Management.md)



# 4. Networking Protocol Design and Data Management

## 4.1 Protocol Selection and Design Rationale

The IoT Smart Lock System employs a hybrid networking approach using MQTT and HTTP to support different communication requirements within the system. This design separates device-level communication from user interaction, improving modularity and system clarity.

MQTT is selected as the primary protocol for communication between the ESP32 and the local IoT gateway due to its lightweight nature, low overhead, and suitability for resource-constrained embedded devices. HTTP is used as a complementary protocol to enable mobile phone–based remote control, providing a simple and widely supported interface for user interaction.

This combination allows efficient real-time data exchange at the device level while maintaining flexibility for external client access.

---

## 4.2 MQTT-Based Device Communication

**Mosquitto MQTT Broker & Node-RED**

The MQTT publish/subscribe protocol is used for communication between the ESP32 smart lock device and the Raspberry Pi gateway. A local MQTT broker is hosted on the Raspberry Pi using Mosquitto MQTT Broker, which manages message routing between publishers and subscribers.

The ESP32 publishes door status updates, authentication results, and access events to predefined MQTT topics. At the same time, it subscribes to specific control topics to receive authorized remote commands forwarded from the application layer. This publish/subscribe model decouples the embedded device from the application logic, allowing data producers and consumers to operate independently.

At the gateway, Node-RED subscribes to the relevant MQTT topics to process incoming messages, implement control logic, and route data to storage and visualization services. MQTT broker authentication is enforced to restrict access to authorized devices within the local network.

---

## 4.3 HTTP-Based Mobile Control

**API, Shortcut Application, Node-RED**

HTTP communication is implemented to support mobile phone–based remote unlocking. An HTTP endpoint is hosted on the Raspberry Pi and managed through Node-RED, allowing control requests to be received from external client devices.

The mobile interface is implemented using the iPhone Shortcuts application, which sends HTTP requests to the local server when a remote unlock action is triggered. Each request includes an API key in the request header, which is validated at the application layer before any control command is accepted.

Upon successful validation, the authorized control command is forwarded internally to the ESP32 through the MQTT channel. This design ensures that direct device access is not exposed to mobile clients, while centralized validation and control are maintained at the gateway.

---

## 4.4 Local Data Handling and Storage

**InfluxDB**

All data handling in this project is performed locally on the Raspberry Pi, without reliance on external cloud services. This local deployment approach reduces external network dependency, minimizes data exposure, and is suitable for prototype evaluation within a controlled local network environment.

Door status updates, authentication events, and access activity logs are stored in InfluxDB, which is optimized for time-series data storage and retrieval. This enables efficient logging of system events for monitoring and analysis purposes.

---

## 4.5 Data Visualization and Dashboard

**Grafana**

Data visualization is implemented using Grafana, which is integrated with the InfluxDB database. Grafana dashboards provide real-time and historical visualization of system activity, including door state changes and access events.

The dashboards allow users to observe system behavior through a web interface hosted on the local server. This visualization approach supports effective monitoring and validation of system functionality during testing and demonstration.

---

Overall, the use of MQTT for device communication, HTTP for mobile interaction, and local data storage and visualization provides a balanced and modular networking design suitable for an IoT smart lock prototype.

---
[Section 5: Security Implementation](/05_Security_Implementation/security.md)

_[Click here to return to Section 3: Implementation](/03_Implementation/implementation.md)_

# 4. Networking Protocol Design and Data Management

## 4.1 Protocol Selection and Design Rationale

The IoT Smart Lock System adopts a hybrid networking architecture using MQTT and HTTP, coordinated through a local gateway implemented with Node-RED on a Raspberry Pi. This design separates device-level communication from user interaction, improving modularity and system clarity.

MQTT is selected for ESP32-to-gateway communication due to its lightweight publish/subscribe model and low overhead, making it suitable for embedded devices. HTTP is employed for mobile interaction (iPhone Shortcut App), providing a standardized and user-friendly interface for remote access and for dashboard viewing (Grafana). Node-RED functions as the central integration layer, enabling controlled interaction between these protocols while maintaining a clear separation of responsibilities.

This combination allows efficient real-time data exchange at the device level while maintaining flexibility for external client access.

---

## 4.2 MQTT-Based Device Communication

**Mosquitto MQTT Broker & Node-RED**

A local MQTT broker (Mosquitto) is hosted on the Raspberry Pi to manage device-level communication. The ESP32 publishes operational data—including door status, lock state, authentication results, and security alerts—to predefined MQTT topics like: "door_status", "lock_status", "door_1/control", and "alert".

All MQTT topics in this implementation are configured using Quality of Service (QoS) level 0 (at most once delivery). The reason this was selected is to minimize latency and protocol overhead, as the system operates within a stable local network environment, also because there are no highly critical or sensitive data being transmitted.

Node-RED subscribes to these topics and performs payload validation, data normalization, and routing. Incoming messages are transformed into a consistent schema and stored in InfluxDB as time-series records. This publish/subscribe approach decouples the embedded device from application logic, enabling scalable and maintainable system behavior.

The figure below is an example of the MQTT node configuration on Node-RED.
![MQTT_Node_Example](https://github.com/user-attachments/assets/5fad8d42-c026-4215-85e7-9733785c763c)

_Figure 2: MQTT In Node-RED Node Example_



---

## 4.3 HTTP-Based Mobile Control via Node-RED

**API, Shortcut Application, Node-RED**

HTTP communication is implemented to support mobile phone–based remote unlocking. HTTP endpoints are hosted on the Raspberry Pi and managed through Node-RED, allowing control requests to be received from external client devices.

The mobile interface is implemented using the iPhone Shortcuts application, which sends HTTP requests to the local server when a remote unlock or get status action is triggered. All HTTP requests require a Bearer API key for authentication. Authorized GET requests retrieve the latest door state from InfluxDB, while authorized POST requests trigger lock or unlock commands. Validated control actions are forwarded internally to the ESP32 using MQTT, ensuring that device-level communication remains isolated from external clients.

The figure below is the shortcut logic used implemented

![Shortcut_App_Logic](https://github.com/user-attachments/assets/5877e6ec-9c1d-42d8-b69c-98e580cf2174)

_Figure 3: Shortcut App Logic_

---

## 4.4 HTTP-Based Dashboard Access and Monitoring

**HTTP, Grafana**
HTTP is also used as the access mechanism for system monitoring and visualization. Grafana is hosted on the Raspberry Pi and accessed through a web-based HTTP interface, allowing users to view real-time door and lock status and historical activity and alert logs from the authorized device within the local network. Through the Grafana dashboards, users can monitor door status changes, lock state transitions, authentication events, and alert records stored in InfluxDB.

## 4.5 Local Data Handling and Storage

**InfluxDB**

All data handling in this project is performed locally on the Raspberry Pi, without reliance on external cloud services. This local deployment approach reduces external network dependency, minimizes data exposure, and is suitable for prototype evaluation within a controlled local network environment.

Door status updates, authentication events, access activity logs, and security alert logs are stored in InfluxDB, which is optimized for time-series data storage and retrieval. This enables efficient logging of system events for monitoring and analysis purposes. Node-RED acts as the data ingestion and preprocessing layer, ensuring that only validated and structured data is written to the database.

---

## 4.5 Data Visualization and Dashboard

**Grafana**

![Grafana_Dashboard](https://github.com/user-attachments/assets/5a0be885-ab7e-4776-bc0d-556e17150cae)

*Figure 4: Grafana Dashboard*


Data visualization is implemented using Grafana, which is integrated with the InfluxDB database. Grafana dashboards provide real-time and historical visualization of system activity, including door and lock state changes, access activity logs, and security alert logs.  

The dashboards allow users to observe system behavior through a web interface hosted on the local server. This visualization approach supports effective monitoring and validation of system functionality during testing and demonstration.

---

## 4.6 Node-RED Flows

![Node_RED_Flow](https://github.com/user-attachments/assets/968b0b18-ba76-4992-9897-acaabe7f7fa1)

_Figure 5: Implemented Node-RED Flow_ 

| Flow Category        | Source                | Protocol     | Endpoint / Topic     | Node-RED Role                  | Data Handling / Outcome                                               |
| -------------------- | --------------------- | ------------ | -------------------- | ------------------------------ | --------------------------------------------------------------------- |
| Door Status Update   | ESP32 (Reed Sensor)   | MQTT (QoS 0) | `door_status`        | Subscribe, validate, normalize | Stores door open/closed state in InfluxDB (`door_status` measurement) |
| Lock Status Update   | ESP32 (Relay)         | MQTT (QoS 0) | `lock_status`        | Subscribe, validate, normalize | Stores locked/unlocked state in InfluxDB (`lock_status` measurement)  |
| Authentication Event | ESP32 (Keypad / RFID) | MQTT (QoS 0) | `authentication_log` | Subscribe, validate            | Logs authentication results in InfluxDB for auditing                  |
| Security Alert       | ESP32                 | MQTT (QoS 0) | `alert`              | Subscribe, validate            | Stores alert messages in InfluxDB (`door_alert` measurement)          |
| Status Query         | Mobile Client         | HTTP GET     | `/control`           | Authenticate request, query DB | Returns latest door status from InfluxDB as JSON                      |
| Remote Control       | Mobile Client         | HTTP POST    | `/control`           | Authenticate, validate command | Forwards lock/unlock command to ESP32 via MQTT                        |
| Control Command      | Node-RED              | MQTT (QoS 0) | `door_1/control`     | Publish command                | ESP32 executes lock or unlock action                                  |
| Dashboard Monitoring | User Browser          | HTTP         | Grafana Web UI       | Serve dashboards               | Displays real-time status, logs, and alerts                           |

---

Overall, the integration of MQTT for device-level communication, HTTP for user interaction, and local data storage and visualization establishes a modular and efficient networking architecture that effectively supports the functional and monitoring requirements of the IoT smart lock system.

---
[Section 5: Security Implementation](/05_Security_Implementation/security.md)

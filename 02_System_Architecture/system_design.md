# 2. System Design and Architecture

## 2.1 Overall System Architecture

The proposed Smart Lock System is based on a four-layer IoT architecture, consisting of the Perception Layer, Network Layer, Application Layer, and Security Layer. 
The system integrates physical sensing and actuation components with network communication, local data processing, and access control mechanisms. A full system architecture diagram illustrates the interaction between hardware components, communication protocols, data paths, and security features across all four layers. The diagram below is the overview of our IoT System Architecture Diagram

<img width="920" height="942" alt="IoT_System_Architecture_Diagram" src="https://github.com/user-attachments/assets/0172fcc8-9963-4759-a7e6-382e891711a2" />

*Diagram 1: IoT System Architecture Diagram*

### Four-Layer Architecture Overview

- **Perception Layer:** Responsible for user authentication, door state sensing, and physical actuation.
- **Network Layer:** Handles communication between the smart lock device and the local gateway.
- **Application Layer:** Performs data processing, storage, visualization, and remote control logic.
- **Security Layer:** Enforces authentication and access control to prevent unauthorized system interaction.

---

## 2.2 Perception Layer (Sensing and Actuation)

The perception layer is implemented using an ESP32 microcontroller, which serves as the primary embedded controller for the smart lock system. This layer is responsible for acquiring input data from authentication devices and sensors, also controlling the door locking mechanism.

The following components are integrated at this layer:

- **RC522 RFID Reader:** Used for contactless authentication by verifying registered UID stored in firmware.
- **3 x 4 Membrane Keypad:** Enables keypad-based PIN authentication for authorized users.
- **Reed Switch:** Detects the physical door state (open / closed) and provides real-time door status feedback.
- **Relay Module and Solenoid Lock:** Acts as the door locking actuator, where the relay is triggered to unlock or lock the door based on authentication results.
- **ESP32 Microcontroller:** Executes authentication logic, processes sensor inputs, controls the actuator, and communicates with the network layer.
- **Buzzer:** Provides audible feedback to alert users regarding prolonged door-open conditions.

The ESP32 will only activate the relay to unlock the door under the following conditions where the predefined authentication and validation conditions are satisfied:
i. **Sucessfull RFID Authentication:**
  The scanned RFID card identifier matches the registered and authorized UID stored in the system.

ii. **Suscessful Keypad PIN Verification:**
  The entered PIN by users matches the predefined balid PIN stored in the ESP32 Firmware.

iii. **Authorized Mobile Phone Remote Command:**
      A remote unlock command is received via the HTTP interface and successfully validated using an API key–based authentication mechanism at the application layer.

After unlocking, the relay may be automatically deactivated after a predefined time interval to relock the door, or once the door state transitions from open to closed. This ensures that the door does not remain unlocked unintentionally.

---

## 2.3 Network Layer (Communication and Connectivity)

The network layer enables data tranmission and exchange between edge devices and the server.
In this this case, ESP32 that serves as the client device gathers the input and sensor data and sends them to Raspberry Pi, that acts as the central IoT gateway. All communication is conducted within a local Wi-Fi network, allowing controlled data exchange between different system components. To support different communication requirements, MQTT and HTTP protocols are used.

i. **MQTT Communication**

The MQTT publish/subscribe protocol is used as the communication mechanism between the ESP32 and the Raspberry Pi. A Mosquitto MQTT broker is hosted on the Raspberry Pi to manage message sending between the publishers and subscribers. 

- The ESP32 publishes door status updates, authentication events, sensor data, and abnormal conditions to predefined MQTT topics (e.g., door_status, lock_status, alert). It also subscribes to a control topic to receive authorized remote commands.
- Node-RED that runs on the Raspberry Pi subscribes to these topic to process incoming data, trigger control logic, and store relevant information in the database for further data visualization using Grafana dashboards.

ii. **HTTP Communication**

The HTTP communication is implemented to support mobile phone-based remote unlocking. An HTTP endpoint is hosted on the Raspberry Pi and managed through Node-RED, allowing external client requests to be received and processed. For the mobile interface, the iPhone Shortcuts application is used to send HTTP requests to the local server to trigger lock and unlock actions. To prevent unauthorized access, each HTTP request requires an API key in the request header that needs to be validated by the server before any control action is executed. Upon successful authentication, the server forwards the authorized control command to the ESP32 through the MQTT channel. Besides, the HTTP protocol is also used to enable the access of the Grafana dashboard for visualization for lock and door status check, along with the door access and security alert logs. 

---

## 2.4 Application Layer (Data Processing and Visualization)

The application layer is hosted on a Raspberry Pi, which functions as both the IoT gateway and local application server. This layer is responsible for processing incoming data, executing control logic, and presenting system information to the user.

Key components at this layer include:

- **Node-RED:** Used for flow-based programming to manage MQTT messages, process authentication events, and handle HTTP control requests from mobile devices.
- **InfluxDB:** A local time-series database used to store door status data and access activity logs for historical analysis and record.
- **Grafana Dashboard:** Provides real-time visualization of door and lock status, access events categorized by authentication along with their success of fail status, and security alert logs for monitoring purposes.

Data from the ESP32 flows through the MQTT broker to Node-RED, where it is processed and conditionally stored in the database or used to trigger alerts and control actions.

---

## 2.5 Security Layer (Access Control and Authorization)

The security layer provides basic but effective access control mechanisms for the system within a controlled local network environment. 

The following security features implemented are:

- **API Key–Based HTTP Authentication:** Mobile phone–based remote unlocking is protected using an API key included in the HTTP request header, which is validated by the application server before executing any control action.
- **MQTT Broker Authentication:** Username and password authentication is enforced at the MQTT broker to ensure that only authorized devices can publish or subscribe to system topics.
- **Device-Level Authentication Logic:** Physical door unlock is only permitted after successful authentication via RFID, PIN entry, or authorized remote command from mobile phone.
- **Host-Level Access Control:** The Raspberry Pi gateway is protected by operating system level authentication as user credentials are required to access the devices. This helps to restrict unauthorized administrative access and configuration to the system services such as the MQTT broker, Node-RED flows, and the local database.

These measures collectively provide layered access control suitable for a prototype deployed in a controlled local network environment.

---

## 2.6 Data Flow Description

The overall data flow of the system can be summarized as follows:

1. **Authentication Request Initiation**  
   The user initiates an access request via RFID, keypad PIN entry, or mobile phone remote control (user interaction at the perception or application layer).

2. **Authentication Validation**  
   The ESP32 validates RFID and keypad credentials locally (device-level authentication), while mobile requests are received by Node-RED and validated using an API key (application-level authentication).

3. **Authorization and Lock Control**  
   If authentication is successful, an authorized unlock command is generated and sent to the ESP32 either locally or via MQTT; otherwise, the request is rejected and logged (access control enforcement).

4. **Door Actuation and Event Generation**  
   The ESP32 actuates the relay to control the solenoid lock and monitors door state changes using the reed switch (physical actuation and sensing).

5. **Data Transmission and Processing**  
   Door status, lock status, authentication events, and alerts are published to the MQTT broker and processed by Node-RED (device-to-gateway communication).

6. **Data Storage and Visualization**  
   Processed events are stored in InfluxDB and visualized in real time on Grafana dashboards for monitoring and historical analysis (application-level data management).

7. **Security Enforcement**  
   Authentication and access control mechanisms are applied throughout the data flow to ensure that only authorized actions and messages are processed (end-to-end security).

This structured data flow demonstrates complete end-to-end integration from sensing and actuation to data management and access control.

**Text-Based Data Flow Diagram**

User  
→ (RFID / Keypad PIN)  
→ ESP32 (Authentication + Actuation)  
→ MQTT Publish (Door Status / Lock Status / Events)  
→ Mosquitto MQTT Broker (Raspberry Pi)  
→ Node-RED (Processing + Control Logic)  
→ InfluxDB (Time-Series Storage)  
→ Grafana Dashboard (Real-Time Visualization)

Mobile Phone (iPhone Shortcuts)  
→ HTTP Request + API Key  
→ Node-RED (API Key Validation)  
→ MQTT Control Command  
→ ESP32 (Remote Unlock Execution)


[Section 3: Implementation](/03_Implementation/implementation.md)






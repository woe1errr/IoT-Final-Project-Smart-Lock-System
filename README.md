# IoT-Final-Project-Smart-Lock-System

## NET3054 Networking and Security Final Project  (*September 2025*)

## [Project Abstract:](00_Abstract/abstract.md)
This project aimed to develop a Multi-Option IoT Smart Lock System that overcomes the limitations of conventional mechanical locks and single-factor digital authentication systems, such as key duplication risks, limited flexibility, and the absence of access monitoring capabilities. The proposed system is a functional prototype that supports multiple authentication options, including RFID tag verification, keypad-based PIN entry, and mobile phone based remote unlocking. The system is designed using a four-layer IoT architecture, where an ESP32 operates at the perception layer for sensing and actuation, while a Raspberry Pi functions as the local server and IoT gateway. At the application layer, Node-RED is employed for flow-based data processing and control logic, while InfluxDB is used for structured storage of access events and lock status data, with these backend services deployed and managed using Docker containers to ensure modularity and ease of deployment. Real-time monitoring and alerting capabilities are enabled through the integrated dashboard. Experimental evaluation demonstrates reliable data transmission between the ESP32 and the gateway, consistent multi-option authentication performance, and timely access notifications, thereby improving both system security and user convenience.

## Documentation Contents
### 1. [Introduction](01_Introduction/introduction.md)
### 2. [Design and System Architecture](02_System_Architecture/system_design.md)
### 3. [Implementation Details](03_Implementation/implementation.md)
### 4. [Networking Protocol](04_Networking_Protocol/Protocol_Design_And_Data_Management.md)
### 5. [Security Implementation](05_Security_Implementation/security.md)
### 6. [Testing and Results](06_Testing_and_Results/testing.md)
### 7. [Conclusion](07_Conclusion_and_Future_Work/conclusion.md)

## Code Files
### 1. [Docker Compose File](IoT_Final_Project_Docker_Compose.yml)
### 2. [ESP32 Firmware Code](IoT_Final_Project_ESP32_Firmware.ino)
### 3. [Grafana Dashboard with Flux Query JSON File](IoT_Final_Project_GrafanaDashboard.json)
### 4. [Node-RED Flow JSON File](IoT_Final_Project_NodeRED_Flow.json)
### 5. [Example of Environment File](IoT_Final_Project_env.example)
### 6. [Example of Mosquitto Configuration File](IoT_Final_Project_mosquitto.conf)
### 5.
### 6. 
Click here to check out the full report in PDF: [IoT Final Project - Lock System Report]()

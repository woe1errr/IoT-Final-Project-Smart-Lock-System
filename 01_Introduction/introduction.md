*[Click here to return to README](/README.md)*

# 1.0 Introduction

## 1.1 Background and Motivation

Access control systems play a critical role in ensuring safety and security in residential environments and shared spaces such as offices, storage rooms, lockers, and campuses. Traditional mechanical lock systems rely on physical keys, which are prone to loss, duplication, theft, and unauthorized copying. These limitations not only reduce user convenience but also introduce significant security risks, as lost or duplicated keys can be exploited without detection.

Although digital locking solutions have been introduced as alternatives, many existing systems rely on a single authentication method, such as a Personal Identification Number (PIN) or an access card. Such single-factor authentication mechanisms still poses vulnerabilities that can potentially allow unauthorized access if credentials are compromised. Furthermore, most traditional lock systems lack real-time access monitoring and alerting capabilities, limiting user awareness and control over access events.

Advancements in Internet of Things (IoT) technologies provide an opportunity to enhance access control systems by enabling connectivity, automation, and real-time monitoring. IoT-based systems allow multiple authentication methods to be integrated into a single platform, while also supporting remote management, access logging, and alert notifications. By connecting physical access devices to a networked application layer, users can gain greater visibility into access activities and exercise improved control, hence enhancing both security and usability.

To overcome these challenges, this project aims to develop an IoT-driven smart lock system that integrates multiple authentication methods, including hardware-based access mechanisms and remote control, with real-time monitoring and user alerting. The proposed system is designed to improve security, flexibility, and convenience through the combination of embedded hardware, networked data management, and basic security controls.


## 1.2 Project Objectives

The primary objectives of this project are as follows:
1. To design and implement a multi-option smart lock system that allows authentication using a keypad (PIN), RFID tag and phone-based unlocking.
2. To establish network communication between the smart lock device and a local IoT gateway using an appropriate communication protocol to control and monitor in real-time.
3. To implement a four-layer IoT architecture integrating perception, network, application, and security layers.
4. To enable real-time monitoring, logging, and visualization of lock status and access events using a local server and database.
5. To incorporate security mechanism to prevent unauthorized access and ensure controlled device communication.


## 1.3 Expected Outcomes

Upon completion of the project, the following outcomes are expected:
- A functional IoT-based smart lock prototype capable of supporting multiple authentication methods, including RFID tag verification, keypad-based PIN entry, and mobile phone–based remote unlocking.
- Successful data transmission between the ESP32 smart lock device and the local IoT gateway within a controlled local network environment.
- Real-time monitoring, logging, and visualization of door lock status and access activities through a Grafana dashboard.
- Application of basic security measures, such as authentication and access control, to restrict unauthorized device interactions.

---

_[Click here to return to Section 2: System Architecture](/02_System_Architecture/system_design.md)_











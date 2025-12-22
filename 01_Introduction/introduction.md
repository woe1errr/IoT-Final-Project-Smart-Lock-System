# 1. Introduction

## 1.1 Background and Motivation

Access control systems are also important in providing safety and security within residential and common areas like offices, storage rooms, lockers and campuses. In the old mechanical types of lock, the physical keys are used and this correlates to the fact that one may easily lose such keys, forget, duplicate or steal. These shortcomings don’t only minimize convenience but also present a great deal of security hazard. Although there are new tools that have been proposed as alternatives to the use of digital locks, a large number of the available options rely on one form of individual authentication, like a Personal Identification Number (PIN) or an access card and are susceptible to unauthorized access in case of credentials theft.

In this case, access control systems may be improved with the development of Internet of Thins (IoT) technologies, which are connected, automated and monitored in real time. IoT allows combining various authentication techniques in one system and providing the ability to manage remotely, recordings and warning systems. Using physical devices to be linked to a networked application layer enables users to be more aware of the events of access and have a greater level of control over the events, enhancing security and usability.

Due to these constraints, this project aims to create an IoT-driven smart lock system, which would allow the integration of multiple ways to unlock the door and real-time monitoring and user alerting. The desired system will enhance security, flexibility, and convenience through hardware-based authentication and networked data management as well as security measures.

---

## 1.2 Project Objectives

The primary objectives of this project are as follows:
1. To design and implement a multi-option smart lock system that allows authentication using a keypad (PIN), RFID tag and phone-based unlocking.
2. To establish network communication between the smart lock device and a local IoT gateway using an appropriate communication protocol to control and monitor in real-time.
3. To implement a four-layer IoT architecture integrating perception, network, application, and security layers.
4. To enable real-time monitoring, logging, and visualization of lock status and access events using a local server and database.
5. To incorporate security mechanism to prevent unauthorized access and ensure controlled device communication.

---

## 1.3 Expected Outcomes

Upon completion of the project, the following outcomes are expected:
- A functional IoT-based smart lock prototype capable of supporting multiple authentication methods, including RFID tag verification, keypad-based PIN entry, and mobile phone–based remote unlocking.
- Successful data transmission between the ESP32 smart lock device and the local IoT gateway within a controlled local network environment.
- Real-time monitoring, logging, and visualization of door lock status and access activities through a Grafana dashboard.
- Application of basic security measures, such as authentication and access control, to restrict unauthorized device interactions.

---

_[Section 2: System Architecture](/02_System_Architecture/system_design.md)_







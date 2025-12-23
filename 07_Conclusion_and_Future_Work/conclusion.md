
 
 # 7.0 Conclusion and Future Work

---

In conclusion, this project has successfully designed and implemented a functional IoT-based Smart Lock System that integrates device-level sensing and actuation, network communication, data management, and basic security mechanisms. The system demonstrated end-to-end operation, from local authentication at the ESP32 device to real-time data visualization and remote control through a secured backend infrastructure.

The figure belows is the final prototype of the smart lock system

![Front_View_of_Prototype](https://github.com/user-attachments/assets/64a8fffe-d5a8-48d7-9a4a-1ec00b92fc75)

*Figure: Front View of Final Prototype*


## 7.1 Key Findings

Throughout the implementation, we have confirmed that lightweight protocols and local gateways are well-suited for smart home security applications. MQTT has enabled low-latency and reliable communication between the ESP32 device and the Node-RED gateway, while HTTP has successfully provided a simple interface for user interaction through a mobile Shortcut application and for dashboard viewing. With the use of a local time-series database and Grafana dashboard, effective real-time monitoring and historical analysis of door activity and alert events were allowed.

From a security perspective, the combination of device-level authentication (PIN and RFID), API key validation for HTTP endpoints, and authenticated MQTT communication provided a basic layered access control approach. 



## 7.2 Reflections on Challenges

Several challenges were encountered during the development and testing phases of the project. At the hardware level, circuit connection and wiring posed initial difficulties. Some ESP32 GPIO pins were already occupied, requiring alternative pin selection and corresponding firmware adjustments. In addition, the large number of jumper wires increased the likelihood of loose connections or incorrect wiring, which occasionally resulted in unexpected behavior during testing and extended debugging time.

From a software and networking perspective, debugging MQTT message formats and database field mappings proved to be non-trivial. Ensuring that data written to the database conformed to Grafana’s time-series requirements required careful validation of measurement names, field types, and timestamps. Misalignment in any of these aspects led to visualization issues that had to be resolved iteratively.

The use of Node-RED also presented a learning challenge, as it was a new development tool for the group. Designing correct function logic, managing message flows, and handling state between different nodes required a period of experimentation and troubleshooting. Similarly, ensuring that HTTP requests were correctly structured and included the required API key headers demanded careful coordination between the mobile Shortcut application and the Node-RED backend.

Despite these challenges, systematic debugging, incremental testing, and increased familiarity with the tools allowed the issues to be resolved successfully. The experience highlighted the importance of careful hardware wiring, structured data design, and thorough understanding of middleware tools in IoT system development.



## 7.3 Future Work and Enhancements

While the current system prototype meets the project objectives, several enhancements could be explored in the future:

- **Transport-Level Security**
  - Implement HTTPS for HTTP communication and MQTT over TLS to ensure data confidentiality and integrity in untrusted networks.

- **Enhanced Authentication and Access Control**
  - Introduce token-based authentication mechanisms (e.g., JWT).
  - Implement role-based access control to differentiate between users (e.g., admin vs. guest).

- **Scalability Improvements**
  - Support multiple smart lock devices with centralized management.
  - Apply more granular MQTT topic structures and access control policies for multi-device environments.

- **Advanced Security Monitoring**
  - Integrate anomaly detection or basic intrusion detection techniques to identify unusual access patterns.
  - Enhance alert logic to include behavioral analysis instead of simple rule-based triggers.

- **Improved User Interface**
  - Replace the iPhone Shortcut control interface with a dedicated mobile or web application for unrestricted usage.
  - Provide richer user feedback, access logs, configuration options through the interface, and to send notifications to users.

- **System Reliability Enhancements**
  - Add offline buffering or retry mechanisms to handle temporary network disruptions.

[Click here to return to README](/README.md)






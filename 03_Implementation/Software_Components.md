| Software Component| Purpose|
| Arduino IDE | Used to develop, compile, and upload firmware to the ESP32 microcontroller, including authentication logic, sensor handling, actuator control, and MQTT communication. |
|Raspberry Pi OS | Operating system that runs on the Raspberry Pi, providing the execution environment for all gateway and application-layer services.|
|Mosquitto MQTT Broker | Acts as the message broker to manage MQTT publish/subscribe communication between the ESP32 and application services.|
|Node-RED| Provides flow-based programming for processing MQTT messages, handling HTTP requests, validating API keys, implementing control logic, and routing data between system components.|
|InfluxDB | Time-series database used to store door status updates, access events, and authentication logs locally for record. |
|Grafana | Web data visualization tool used to display real-time and historical data such as door state changes and access activities through dashboards.|
|iPhone Shortcuts App| Acts as the mobile client interface, allowing users to trigger remote unlock actions by sending HTTP requests with API key authentication headers.|

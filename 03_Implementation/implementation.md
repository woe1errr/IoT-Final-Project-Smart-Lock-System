# 3. Implementation Details

## 3.1 Hardware Implementation and Circuit Connections

The hardware implementation of the IoT Smart Lock System is centered around the ESP32 microcontroller, which connects to multiple sensing, authentication, alert, and actuation components. The prototyping, testing and debugging is done on a breadboard where all the components are assembled.

The **RC522 RFID reader** is connected to the ESP32 using the SPI communication interface to enable contactless RFID-based authentication. A **3×4 membrane keypad** is connected via digital GPIO pins to support PIN-based authentication. User prompts, system status messages, and authentication feedback are displayed on an **I2C LCD display**, which reduces GPIO usage by utilizing the I2C communication protocol.

A **magnetic reed switch** is installed on the door frame to detect whether the door is open or closed. The reed switch output is monitored continuously by the ESP32 to support real-time door state tracking and door open escalation logic. An **active buzzer** is connected to a digital output pin to give immediate audio that would alert the users in conditions regarding prolonged door escalation time and repeated failed attempts.  

The **solenoid door lock** serves as the primary physical locking mechanism. The solenoid runs on a greater current than the ESP32 can comfortably deliver, so a **5V relay module** has been used to provide electrical isolation and control of the solenoid.  **18650 battery and battery holder** are also used to provide adequate power to the solenoid lock.

For a clearer overview of the hardware components and their respective purposes, refer to the [Hardware Components Table](./Hardware_Components.md).

The figure below shows the circuit connection diagram of the prototype. Detailed wiring information can be found in the [Wiring Table](./Wiring_Table.md).


<img width="3000" height="2919" alt="Circuit_Connection" src= "https://github.com/woe1errr/IoT-Final-Project-Smart-Lock-System/blob/main/Diagram/Circuit_Connection_Diagram.png">

*Diagram 2: Circuit Connection Diagram*

![Actual_Prototype_Wiring](https://github.com/user-attachments/assets/3ba76d14-efad-4033-8d04-71a0bbfd1ebe)

*Figure 1: Actual Prototype Wiring Image*

---

## 3.2 Firmware Design and Core Modules

The firmware for the ESP32 is developed using the Arduino IDE and written in C++. For better clarity, the code is divided into several modules. Besides, all authentication checks are performed locally on the ESP32 before any physical actuation occurs, ensuring that unauthorized inputs do not trigger the relay to activate the solenoid lock.

The core firmware modules include:

- **Authentication Module:**  
  This module handles RFID tag verification and keypad-based PIN validation. Only the tag with the correct predefined UID and authenticated inputs are allowed to move on with the lock unlock logic.

```
const String correctPIN = "1234";
const int PIN_length = 4;
String correctUID[] = { "b0d95222" };

loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    cardActive = true;

    String uidString = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
      uidString += String(mfrc522.uid.uidByte[i], HEX);
    }

    Serial.print("RFID UID: ");
    Serial.println(uidString);

    bool authorized = false;
    for (const auto& UID : correctUID) {
      if (uidString == UID) {
        authorized = true;
        break;
      }
    }

    if (authorized) {
      unlockDoor("RFID");
    } else {
      displayError("Invalid Card");
      sendLogToMQTT(false, "RFID", true);
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

char key = keypad.getKey();
  if (key) {
    Serial.println(key);
    
    // Clear input
    if (key == '*') {
      enteredPIN = "";
      resetDisplay();
      Serial.println("PIN cleared");
      return;
    }

    // Submit PIN
    if (key == '#') {
      if (enteredPIN.length() == PIN_length) {
        if (enteredPIN == correctPIN) {
          Serial.println("Door unlocked");
          unlockDoor("PIN");
        } else {
          Serial.println("Wrong PIN. Try again");
          displayError("Wrong PIN");
          sendLogToMQTT(false, "PIN", true);
          failedAttempts += 1;

          if (failedAttempts > 2) {
            sendAlertToMQTT(
              "Invalid PIN for " + String(failedAttempts) + " times"
            );
          }
        }
      } else {
        Serial.println("PIN too short");
        displayError("PIN too short");
      }

      enteredPIN = "";   // reset after submit
      return;
    }

    if (enteredPIN.length() < PIN_length) {
      enteredPIN += key;
      displayPinNumber();
      Serial.print("PIN so far: ");
      for (int i = 0; i < enteredPIN.length(); i++) Serial.print("*");
      Serial.println();
    }
  }
}
```

- **Sensor Monitoring Module:**
  The magnetic reed switch constantly monitors the door's open or closed states and generates corresponding status updates.

```
int sensorState = 0;
int lastSensorState = 0;

loop() {
  sensorState = digitalRead(REED_SENSOR_PIN);
  if (sensorState != lastSensorState) {
    if (sensorState == LOW) {
      Serial.println("Door is Closed!");
      sendReedToMQTT(false);
      doorOpenTimerActive = 0;
      digitalWrite(BUZZER_PIN, LOW);
      doorOpenedTimedoutAlertSent = 0;
    } else {
      Serial.println("Sensor Opened!");
      doorShouldLockMills = millis() + 1000;
      doorLeftOpenedBeep = millis() + 5000;
      doorOpenTimerActive = 1;
      sendReedToMQTT(true);
    }
    lastSensorState = sensorState;
  }
}
```

- **Actuation Module:**  
  Controls the relay module to unlock or relock the solenoid door lock based on the authentication results and time escalation logic.

```
void unlockDoor(String method) {
  digitalWrite(RELAY_PIN, HIGH);
  doorShouldLockMills = millis() + 7000;
  enteredPIN = "";
  cardActive = false;
  if (lockState == 0) {
    lockState = 1;
    failedAttempts = 0;
    displayDoorUnlocked();
    sendLockToMQTT(true);
    if (method != "") {
      sendLogToMQTT(true, method, true);
    }
  }
}

void lockDoor(String method) {
  digitalWrite(RELAY_PIN, LOW);
  lockState = 0;
  cardActive = false;
  resetDisplay();
  sendLockToMQTT(false);
  if (method != "") {
    sendLogToMQTT(true, method, false);
  }
}
```

- **Buzzer Control Module:**  
  To generate audible alerts to attract user's attention when the door being open exceeds the allowed time threshold.

```
unsigned long doorLeftOpenedBeep = 0;
int doorOpenTimerActive = 0;
int doorOpenedTimedoutAlertSent = 0;

bool deadlinePassed(unsigned long deadline) {
  return (long)(millis() - deadline) >= 0;
}

loop() {
  sensorState = digitalRead(REED_SENSOR_PIN);
  if (sensorState != lastSensorState) {
    if (sensorState == LOW) {
      Serial.println("Door is Closed!");
      sendReedToMQTT(false);
      doorOpenTimerActive = 0;
      digitalWrite(BUZZER_PIN, LOW);
      doorOpenedTimedoutAlertSent = 0;
    } else {
      Serial.println("Sensor Opened!");
      doorShouldLockMills = millis() + 1000;
      doorLeftOpenedBeep = millis() + 5000;
      doorOpenTimerActive = 1;
      sendReedToMQTT(true);
    }
    lastSensorState = sensorState;
  }

  if (doorOpenTimerActive == 1 && deadlinePassed(doorLeftOpenedBeep)) {
    digitalWrite(BUZZER_PIN, HIGH);
    if (doorOpenedTimedoutAlertSent == 0) {
      sendAlertToMQTT(
          "Door Opened for More than 5 seconds"
      );
      doorOpenedTimedoutAlertSent = 1;
    }
  }
}
```

- **Communication Module:**  
  Manages Wi-Fi connectivity, MQTT client initialization, message publishing for door status and access events, and subscription to control topics for authorized remote commands.

```
const char* ssid = "";
const char* password = "";

const char door_id[] = "door_1";

const char MQTT_BROKER_ADRRESS[] = "172.20.10.3";
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "door_1_esp";
const char MQTT_USERNAME[] = "door_1";
const char MQTT_PASSWORD[] = "r";

const char SUBSCRIBE_TOPIC[] = "door_1/control";

MFRC522DriverPinSimple ss_pin(5);
MFRC522DriverPinSimple rst_pin(4);

MFRC522DriverSPI driver{ss_pin}; // Create SPI driver
MFRC522 mfrc522{driver};         // Create MFRC522 instance

WiFiClient network;
MQTTClient mqtt = MQTTClient(256);

void ConnectedToAP_Handler(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
  Serial.println("Connected To The WiFi Network");
}
 
void GotIP_Handler(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}
 
void WiFi_Disconnected_Handler(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
  Serial.println("Disconnected From WiFi Network");
  // Attempt Re-Connection
  WiFi.begin(ssid, password);
}

void connectToMQTT() {
  // Connect to the MQTT broker
  mqtt.begin(MQTT_BROKER_ADRRESS, MQTT_PORT, network);

  // Create a handler for incoming messages
  mqtt.onMessage(messageHandler);

  Serial.print("Connecting to MQTT broker");

  while (!mqtt.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if (!mqtt.connected()) {
    Serial.println("MQTT broker Timeout!");
    return;
  }

  // Subscribe to a topic, the incoming messages are processed by messageHandler() function
  if (mqtt.subscribe(SUBSCRIBE_TOPIC))
    Serial.print("Subscribed to the topic: ");
  else
    Serial.print("Failed to subscribe to the topic: ");

  Serial.println(SUBSCRIBE_TOPIC);
  Serial.println("ESP32 - MQTT broker Connected!");
}

void messageHandler(String &topic, String &payload) {
  Serial.println("ESP32 - received from MQTT:");
  Serial.println("- topic: " + topic);
  Serial.println("- payload:");
  Serial.println(payload);

  // Parse JSON
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("JSON parse failed: ");
    Serial.println(error.c_str());
    return;   // hard stop
  }

  // Validate field
  if (!doc.containsKey("action")) {
    Serial.println("Missing 'action' field");
    return;
  }

  int action = doc["action"];

  // Control logic
  if (action == 1) {
    Serial.println("MQTT ACTION: UNLOCK DOOR");
    unlockDoor("");
  } 
  else if (action == 0) {
    lockDoor("");
  } else {
    Serial.println("Invalid action value from MQTT");
  }
}


void sendReedToMQTT(bool status) {
  StaticJsonDocument<200> doc;
  doc["status"]  = status ? "open" : "closed";
  doc["door_id"] = door_id;

  char payload[256];
  size_t len = serializeJson(doc, payload, sizeof(payload));

  if (len == 0) {
    Serial.println("JSON serialization failed");
    return;
  }

  const char* topic = "door_status";
  bool ok = mqtt.publish(topic, payload, len);

  if (!ok) {
    Serial.println("MQTT publish failed");
    return;
  }

  Serial.println("== MQTT publish successful ==");
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.println(payload);
}

void sendLockToMQTT(bool status) {
  StaticJsonDocument<200> doc;
  doc["status"]  = status ? "unlocked" : "locked";
  doc["door_id"] = door_id;

  char payload[256];
  size_t len = serializeJson(doc, payload, sizeof(payload));

  if (len == 0) {
    Serial.println("JSON serialization failed");
    return;
  }

  const char* topic = "lock_status";
  bool ok = mqtt.publish(topic, payload, len);

  if (!ok) {
    Serial.println("MQTT publish failed");
    return;
  }

  Serial.println("== MQTT publish successful ==");
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.println(payload);
}

void sendAlertToMQTT(String message) {
  StaticJsonDocument<200> doc;
  doc["message"]  = message;
  doc["door_id"] = door_id;

  char payload[256];
  size_t len = serializeJson(doc, payload, sizeof(payload));

  if (len == 0) {
    Serial.println("JSON serialization failed");
    return;
  }

  const char* topic = "alert";
  bool ok = mqtt.publish(topic, payload, len);

  if (!ok) {
    Serial.println("MQTT publish failed");
    return;
  }

  Serial.println("== MQTT publish successful ==");
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.println(payload);
}

void sendLogToMQTT(bool success, String method, bool action) {
  StaticJsonDocument<200> doc;
  doc["status"]  = success ? "success" : "failed";
  doc["door_id"] = door_id;
  doc["method"] = method;
  doc["action"] = action ? "unlock" : "lock";

  char payload[256];
  size_t len = serializeJson(doc, payload, sizeof(payload));

  if (len == 0) {
    Serial.println("JSON serialization failed");
    return;
  }

  const char* topic = "authentication_log";
  bool ok = mqtt.publish(topic, payload, len);

  if (!ok) {
    Serial.println("MQTT publish failed");
    return;
  }

  Serial.println("== MQTT publish successful ==");
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.println(payload);
}

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.onEvent(ConnectedToAP_Handler, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(GotIP_Handler, ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFi_Disconnected_Handler, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi Network ..");
  connectToMQTT();
}

void loop() {
  if (doorOpenTimerActive == 1 && deadlinePassed(doorLeftOpenedBeep)) {
    digitalWrite(BUZZER_PIN, HIGH);
    if (doorOpenedTimedoutAlertSent == 0) {
      sendAlertToMQTT(
          "Door Opened for More than 5 seconds"
      );
      doorOpenedTimedoutAlertSent = 1;
    }
  }

  mqtt.loop();
}
```

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







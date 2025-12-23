// Libraries
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <string>
#include <vector>
#include <WiFi.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>

const char* ssid = "";
const char* password = "";

const char door_id[] = "door_1";

const char MQTT_BROKER_ADRRESS[] = "172.20.10.3";
const int MQTT_PORT = 1883;
const char MQTT_CLIENT_ID[] = "door_1_esp";
const char MQTT_USERNAME[] = "door_1";
const char MQTT_PASSWORD[] = "";

const char SUBSCRIBE_TOPIC[] = "door_1/control";

const int PUBLISH_INTERVAL = 5000;

// Define Hardware Pins
#define KEYPADROWS  4
#define KEYPADCOLS  3
#define RELAY_PIN 17
#define REED_SENSOR_PIN 15
#define MAX_PIN_LEN 17
#define BUZZER_PIN 16

uint8_t rowPins[KEYPADROWS] = {14, 27, 26, 25}; // GIOP14, GIOP27, GIOP26, GIOP25
uint8_t colPins[KEYPADCOLS] = {33, 32, 13};     // GIOP33, GIOP32, GIOP13
 
// ===============================================================
//                         DEFINE VARIABLES
// ===============================================================
// Keypad
char keyMap[KEYPADROWS][KEYPADCOLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

Keypad keypad = Keypad(makeKeymap(keyMap), rowPins, colPins, KEYPADROWS, KEYPADCOLS );

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// LCD Display
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

MFRC522DriverPinSimple ss_pin(5);
MFRC522DriverPinSimple rst_pin(4);

MFRC522DriverSPI driver{ss_pin}; // Create SPI driver
MFRC522 mfrc522{driver};         // Create MFRC522 instance

WiFiClient network;
MQTTClient mqtt = MQTTClient(256);

// ===============================================================
//                   Authentication Variables
// ===============================================================
String enteredPIN = "";
char masked[20];
int failedAttempts = 0;
const String correctPIN = "1234";
const int PIN_length = 4;
String correctUID[] = { "b0d95222" };

int sensorState = 0;
int lastSensorState = 0;
unsigned long doorShouldLockMills = 0;
int lockState = 0;
unsigned long displayShouldResetMills = 0;
int displayResetState = 0;
unsigned long lastPublishTime = 0;
bool cardActive = false;
unsigned long doorLeftOpenedBeep = 0;
int doorOpenTimerActive = 0;
int doorOpenedTimedoutAlertSent = 0;


// ===============================================================
//                   WIFI Helper Functions
// ===============================================================

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

// ===============================================================
//                   MQTT Helper Functions
// ===============================================================

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

// ===============================================================
//                   Display Helper Function
// ===============================================================

void resetDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter PIN / Wave");
  lcd.setCursor(0,1);
  lcd.print("Card to unlock");
  displayResetState = 1;
  enteredPIN = "";
}

void displayPinNumber() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PIN:");
  lcd.setCursor(0,1);
  int len = enteredPIN.length();
  if (len > MAX_PIN_LEN) {
    len = MAX_PIN_LEN;
  }
  for (int i = 0; i < len; i++) {
    masked[i] = '*';
  }
  masked[len] = '\0';
  lcd.print(masked);
  displayResetState = 0;
  displayShouldResetMills = millis() + 10000;
}

void displayError(String message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Error");
  lcd.setCursor(0,1);
  lcd.print(message);
  displayResetState = 0;
  displayShouldResetMills = millis() + 5000;
}

void displayDoorUnlocked() {
  // Update LCD Screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Door Unlocked");
  displayResetState = 0;
  displayShouldResetMills = millis() + 10000;
}

// ===============================================================
//                   Lock Control Helper Functions
// ===============================================================

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

bool deadlinePassed(unsigned long deadline) {
  return (long)(millis() - deadline) >= 0;
}

// ===============================================================
//                         SETUP
// ===============================================================
void setup() {
  // Setup Relay and Reed Sensor
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(REED_SENSOR_PIN, INPUT_PULLUP);

  digitalWrite(BUZZER_PIN, HIGH);

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  Serial.begin(115200);  // Initialize serial communication
  while (!Serial);       // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4).
  // I2C init (LCD)
  Wire.begin(21, 22);   // SDA, SCL

  // SPI init (RFID)
  SPI.begin(18, 19, 23, 5);
  // SCK, MISO, MOSI, SS
  
  mfrc522.PCD_Init();    // Init MFRC522 board.
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);	// Show details of PCD - MFRC522 Card Reader details.
  Serial.println(F("Scan PICC to see UID"));
  // print message
  resetDisplay();
  analogSetAttenuation(ADC_11db);
  WiFi.mode(WIFI_STA);
  WiFi.onEvent(ConnectedToAP_Handler, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(GotIP_Handler, ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFi_Disconnected_Handler, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi Network ..");
  connectToMQTT();
}

void loop() {
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

  if (lockState == 1 && deadlinePassed(doorShouldLockMills)) {
    lockDoor("");
  }

  if (deadlinePassed(displayShouldResetMills) && displayResetState == 0) {
    resetDisplay();
  }

  if (cardActive && !mfrc522.PICC_IsNewCardPresent()) {
    cardActive = false;
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

  mqtt.loop();
}

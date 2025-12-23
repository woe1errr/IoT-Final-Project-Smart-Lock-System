# Testing and Results
---

In this section, the results obtained from the testing procedures on the IoT Smart Lock System are presented, and the system's performance and limitations are discussed.

---

## 7.1 Testing Procedure

System testing was conducted in a controlled local network environment to verify correct operation of authentication mechanisms, network communication, data logging, visualization, and security controls. The tests focused on validating end-to-end data flow from the ESP32 device to the application layer, as well as ensuring unauthorized access attempts were correctly blocked.

The following test cases in Section 7.2 are executed to test system behaviour.

---

## 7.2 Test Case Summary and Results

| **Test ID** | **Test Scenario**                 | **Input / Action**                | **Expected Outcome**               | **Observed Result**                       |
| ----------- | --------------------------------- | --------------------------------- | ---------------------------------- | ----------------------------------------- |
| TC-01       | Valid PIN authentication          | Enter correct PIN on keypad       | Door unlocks and status is updated | Door unlocked successfully; status logged |
| TC-02       | Invalid PIN attempt               | Enter incorrect PIN               | Access denied; no unlock           | Access denied; failed attempt recorded    |
| TC-03       | Multiple invalid PIN attempts     | Enter wrong PIN repeatedly        | Alert event generated              | Alert published and logged                |
| TC-04       | Valid RFID authentication         | Scan registered RFID card         | Door unlocks and status updated    | Door unlocked successfully                |
| TC-05       | Invalid RFID authentication       | Scan unregistered RFID card       | Access denied                      | Access denied; no state change            |
| TC-06       | MQTT status publish               | ESP32 publishes door status       | Message received by Node-RED       | Message received and processed            |
| TC-07       | MQTT authentication failure       | Connect with invalid credentials  | Broker rejects connection          | Connection rejected                       |
| TC-08       | HTTP control with valid API key   | HTTP request with correct API key | Command accepted and executed      | Lock controlled successfully              |
| TC-09       | HTTP control without API key      | HTTP request without API key      | Request rejected                   | Request rejected                          |
| TC-10       | HTTP control with invalid API key | HTTP request with wrong API key   | Request rejected                   | Request rejected                          |
| TC-11       | Data logging                      | Door open/close events            | Data stored in database            | Data visible in dashboard                 |
| TC-12       | Real-time visualization           | Change door/lock state            | Dashboard updates in real time     | Dashboard updated correctly               |



[Section 7: Conclusion and Future Work](/07_Conclusion_and_Future_Work/conclusion.md)


_[Click here to return to Section 5: Security Implementation](/05_Security_Implementation/security.md)_

 # 6.0 Testing and Results
---

In this section, the results obtained from the testing procedures on the IoT Smart Lock System are presented, and the system's performance and limitations are discussed.

---

## 6.1 Testing Procedure

System testing was conducted in a controlled local network environment to verify correct operation of authentication mechanisms, network communication, data logging, visualization, and security controls. The tests focused on validating end-to-end data flow from the ESP32 device to the application layer, as well as ensuring unauthorized access attempts were correctly blocked.

The following test cases in Section 7.2 are executed to test system behaviour.

---

## 6.2 Test Case Summary and Results

| **Test ID** | **Test Scenario**                 | **Input / Action**                | **Expected Outcome**               | **Observed Result**                       |
| ----------- | --------------------------------- | --------------------------------- | ---------------------------------- | ----------------------------------------- |
| TC-01       | Valid PIN authentication          | Enter correct PIN on keypad       | Door unlocks and status is updated | Door unlocked successfully; status logged |
| TC-02       | Invalid PIN attempt               | Enter incorrect PIN               | Access denied; no unlock           | Access denied; failed attempt recorded    |
| TC-03       | Multiple invalid PIN attempts     | Enter wrong PIN repeatedly        | Alert event generated              | Alert published and logged                |
| TC-04       | Valid RFID authentication         | Scan registered RFID card         | Door unlocks and status updated    | Door unlocked successfully                |
| TC-05       | Invalid RFID authentication       | Scan unregistered RFID card       | Access denied                      | Access denied; no state change            |
| TC-06       | MQTT status publish               | ESP32 publishes door status       | Message received by Node-RED       | Message received and processed            |
| TC-07       | HTTP control with valid API key   | HTTP request with correct API key | Command accepted and executed      | Lock controlled successfully              |
| TC-08       | HTTP control with invalid API key | HTTP request with wrong API key   | Request rejected                   | Request rejected                          |
| TC-09       | Data logging                      | Door open/close events            | Data stored in database            | Data visible in dashboard                 |
| TC-10       | Real-time visualization           | Change door/lock state            | Dashboard updates in real time     | Dashboard updated correctly               |


**Test Case Evidences:**
- **TC-01**
  
  ![TC-01](https://github.com/user-attachments/assets/e880777e-1802-4aa9-98f2-5a58f8b42430)
  Figure 15: Evidence for Test Case 1

- **TC-02**
  
![TC-02](https://github.com/user-attachments/assets/453ecdcf-c682-4aa1-8c42-1d1687d35801)

Figure 16: Evidence for Test Case 2

- **TC-03**

  ![TC-03](https://github.com/user-attachments/assets/7dba76e9-9154-4852-9e34-a32cba8153b3)

 Figure 17: Evidence for Test Case 3
 
- **TC-04**

![TC-04](https://github.com/user-attachments/assets/f49269f1-52ee-4ca5-88f3-83345b50a72d)

  Figure 18: Evidence for Test Case 4
  
- **TC-05**

  ![TC-05](https://github.com/user-attachments/assets/f78dd0d2-d8bd-455a-9a5d-d1623542cd0c)

  Figure 19: Evidence for Test Case 5
  
- **TC-06**

  ![TC-06](https://github.com/user-attachments/assets/76aba706-5bb7-449a-a56f-d66abf063249)

 Figure 20: Evidence for Test Case 6

- **TC-07**

![TC-08](https://github.com/user-attachments/assets/aa12164a-1332-487d-b41c-38c2ab909756)

  Figure 21: Evidence for Test Case 7
  
- **TC-08**
  
![Wrong_API_Key](https://github.com/user-attachments/assets/bbac3e79-c49f-4bc3-af3a-f1c660ede331)

  Figure 22: Evidence for Test Case 8
  
- **TC-09, TC-10**

  ![Grafana_Dashboard](https://github.com/user-attachments/assets/0a31b485-f059-4d50-9ea0-02b8942b2cfe)

  Figure 23: Evidence for Test Case 10 and 11_1

![TC-09](https://github.com/user-attachments/assets/6fddfa81-89e4-4273-825d-5b1b0a0cfe35)

  Figure 24: Evidence for Test Case 10 and 11_2


---

## 6.3 System Performance Discussion

The system demonstrated stable and responsive behavior during testing:

- Responsiveness:
  MQTT-based communication enabled low-latency transmission of door and lock status updates. Meanwhile, the dashboard results in near real-time updates but still shows latency for a few seconds for it to refresh when data comes in.

- Reliability:
  Repeated testing showed consistent operation with no unexpected lock activations or missed status messages under normal usage conditions.

- Basic Security Enforcement:
  API key validation and MQTT authentication effectively prevented unauthorized control.
  
---

## 6.4 Limitations
Despite the successed functionalities, several limitations were identified in this system:

1. No Transport-Level Encryption:
MQTT and HTTP communications are not encrypted, limiting suitability for deployment over public or untrusted networks.

2. Limited Scalability Testing:
The system was tested with a single smart lock device; performance under multiple concurrent devices was not evaluated.

3. Basic Alert Logic:
Alert generation is rule-based and does not include advanced intrusion detection or anomaly analysis.

4. Dependence on Network Availability:
Temporary network disruptions may delay data updates or remote control actions.

[Section 7: Conclusion and Future Work](/07_Conclusion_and_Future_Work/conclusion.md)









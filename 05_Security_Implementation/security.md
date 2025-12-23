_[Click here to return to Section 4: Networking Protocol](/04_Networking_Protocol/Protocol_Design_And_Data_Management.md)_
 
 # 5.0 Security Mechanism

To ensure the confidentiality, integrity, and controlled access of the IoT Smart Lock System, three security mechanisms were implemented across the application, network, and device levels. .

---
## 5.1 Implemented Security Mechanisms

### **i. HTTP API Key Validation (Node-RED)**

To secure HTTP-based control and query endpoints, an API key–based access control mechanism was implemented at the application layer using Node-RED. The API key is defined as an environment variable (API_KEY) within the Node-RED runtime settings, ensuring that sensitive credentials are not hardcoded into the flow logic.

![API_Key_Env_Var](https://github.com/user-attachments/assets/27fb8e7f-9be7-4c6d-a70b-c9fa7f1cd5f5)

_Figure 6: API Key in Environment Variable of Node-RED_


When an HTTP request is received, Node-RED extracts the "Authorization" header and validates it against the stored API_KEY environment variable. Requests with missing or mismatched API keys are immediately rejected, and no control or data processing logic is executed. Only requests presenting a valid API key are allowed to proceed to downstream nodes.

On the client side, the mobile Shortcut application is configured to include the API key in the HTTP request headers. The "Authorization" header is explicitly added, with its value set to the same API key configured in Node-RED. This ensures that only trusted client shortcuts can invoke lock control or status query operations.

![API_Key_Shortcut_App](https://github.com/user-attachments/assets/10fce34c-f678-410f-88c4-5445de3af01b)

_Figure 7: API Key in Shortcut App_


This mechanism effectively prevents unauthorized access to the HTTP interface, reduces exposure of critical control endpoints, and provides a simple yet effective access control layer suitable for a local IoT prototype deployment.

**Evidence of Operation**
1. Unauthorized HTTP requests without a valid API key return an error response.
   As from the image, if the incorrect API key is provided, the server will return "Invalid API Key" error message instead of the door status
   
   ![Wrong_API_Key](https://github.com/user-attachments/assets/3d37ed0c-6783-45e4-8d74-daca074a0142)
   
   *Figure 8: Error Response for Wrong API Key*

3. Authorized requests with the correct API key successfully trigger control flows in Node-RED.
4. 
   ![Valid_API_Key](https://github.com/user-attachments/assets/9932f34c-3b06-45a8-95da-42b697456382)
   
  *Figure 9: Valid API Key Triggering Control Flow*
   
---

### **ii. Authentication Logic (PIN and RFID)**

At the device layer, behavioural security is implemented through user authentication by PIN-based input via a keypad and RFID card verification. The ESP32 firmware validates user credentials locally before performing any lock actuation. Only predefined PIN codes and registered RFID UID are accepted.

To enhance security, incorrect authentication attempts are tracked, and repeated failed attempts are published to the backend system and logged into the database to be reflected on the dashboard. Besides, 

**Evidence of Operation**

1. Correct PIN or RFID input results in successful unlock events.
   
![Successful_PIN_RFID_Attempts](https://github.com/user-attachments/assets/20b29931-3233-4c4b-820e-e3eb7a24b5e7)

*Figure 10: Successful Unlock Events for PIN and RFID Input*

2. Invalid credentials are rejected, with failed attempts logged.
   
![Failed_Attempt_Logged](https://github.com/user-attachments/assets/52fb51c7-58dc-4495-b302-1d2391d5f66b)

*Figure 11: Failed Attempts for RFID and PIN Logged*
   
3. Alert messages are published when repeated authentication failures occur.
   
![Repeated_Failed_Attempts](https://github.com/user-attachments/assets/8a7b328d-99c9-44ad-813f-7ccad71a4173)

*Figure 12: Repeated Failed PIN Input Attempts Logged*

---

### **iii. MQTT Authentication and Topic Restriction**
For device-to-server communication, MQTT authentication is enforced using a username and password configured on the broker. Each ESP32 device connects using unique credentials and is restricted to publishing and subscribing only to predefined topics related to its assigned door identifier.

This prevents unauthorized MQTT clients from injecting false data or issuing control commands. Additionally, topic-level separation ensures that devices cannot interfere with each other’s messages, improving system isolation and integrity

**Evidence of Operation**
1. MQTT clients without valid credentials are rejected by the broker.
   
   ![Correct_MQTT_Password](https://github.com/user-attachments/assets/467b6b96-2b45-4915-9fd7-f9d597667690)
   *Figure 13: Correct MQTT Password*

   ![Incorrect_MQTT_Password](https://github.com/user-attachments/assets/2c39df3f-7c58-46c6-b76e-411352649709)
   
   *Figure 14: Wrong MQTT Password*

---

Overall, the implemented security mechanisms collectively protect the system at multiple layers: HTTP access is controlled through API keys, physical access is enforced through local authentication logic, and network communication is secured via authenticated MQTT sessions with topic restrictions. While advanced encryption mechanisms such as TLS are identified as future enhancements, the current implementation demonstrates a clear and functional understanding of IoT security principles suitable for a prototype level system.


[Section 6: Testing and Results](/06_Testing_and_Results/testing.md)






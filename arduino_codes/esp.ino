#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "running_rat";
const char* password = "00001111One!";

WebServer server(80);
String fingerprintStatus = "Waiting for data...";
String objectStatusMessage = "Waiting for object data...";
String gsmStatusMessage = "Waiting for GSM alerts...";
String statusMessage = "Waiting for data...";

void handleRoot() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "text/plain", statusMessage);
}

void handleRegister() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    fingerprintStatus = "Registering Fingerprint...";
    server.send(200, "text/plain", "Registering...");
    Serial2.println("E"); 
}

void handleObjectData() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "text/plain", objectStatusMessage);
}

void handleGSMData() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(200, "text/plain", gsmStatusMessage);
}

void setup() {
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, 16, 17); 

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/data", HTTP_GET, handleRoot);
    server.on("/register", HTTP_POST, handleRegister);
    server.on("/object", HTTP_GET, handleObjectData);
    server.on("/gsm", HTTP_GET, handleGSMData);

    server.on("/register", HTTP_OPTIONS, []() {
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
        server.send(204); 
    });

    server.begin();
}

void loop() {
    server.handleClient(); 

    if (Serial2.available()) {
        String receivedData = Serial2.readStringUntil('\n'); 
        receivedData.trim(); 

        Serial.print("Received from Uno: ");
        Serial.println(receivedData);
        
        // para sa gms_ultrasonic_sensor
        if (receivedData == "Sensor trigger sending Message!") {
            objectStatusMessage = "Sensor trigger sending Message!"; 
        } else if (receivedData == "out of range") {
            objectStatusMessage = "No object detected.";
        }
        
        // para sa ma rrecieved text
        else if (receivedData.startsWith("Alert:")) {
            gsmStatusMessage = receivedData;
        }
        else if (receivedData == "standby to send message") { 
            gsmStatusMessage = "standby to send message"; 
        }
        
        // para sa Fingerprint status
        else if (receivedData == "MATCH/Door Opened") { 
            fingerprintStatus = "MATCH/Door Opened"; 
        } else if (receivedData == "NO MATCH/Door Closed") {
            fingerprintStatus = "NO MATCH/Door Closed"; 
        }
        
        // para sa enrollment ng Fingerprint
        else if (receivedData == "SUCCESS: Fingerprint stored") { 
            fingerprintStatus = "SUCCESS: Fingerprint stored"; 
        } else if (receivedData == "ERROR: Storing failed") {
            fingerprintStatus = "ERROR: Storing failed"; 
        }

        statusMessage = receivedData;
        Serial.print("Updated status: ");
        Serial.println(statusMessage);
    }
}

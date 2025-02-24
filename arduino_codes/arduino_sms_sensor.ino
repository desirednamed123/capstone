#include <SoftwareSerial.h>

// GSM Module
SoftwareSerial sim(6, 7); 

// Ultrasonic Sensor Pins
#define trigPin1 8
#define echoPin1 9
#define trigPin2 10
#define echoPin2 11
#define trigPin3 12
#define echoPin3 13

// Phone number to send alerts
const char* phoneNumber = "+639123456789";

void setup() {
    Serial.begin(9600);  // Communication with Serial Monitor
    sim.begin(9600);     // GSM module communication

    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    pinMode(trigPin3, OUTPUT);
    pinMode(echoPin3, INPUT);
}

void loop() {
    int distance1 = getDistance(trigPin1, echoPin1);
    int distance2 = getDistance(trigPin2, echoPin2);
    int distance3 = getDistance(trigPin3, echoPin3);

    Serial.print("Sensor 1: "); Serial.print(distance1); Serial.print(" cm, ");
    Serial.print("Sensor 2: "); Serial.print(distance2); Serial.print(" cm, ");
    Serial.print("Sensor 3: "); Serial.print(distance3); Serial.println(" cm.");

    if (distance1 <= 10 || distance2 <= 10 || distance3 <= 10) {
        Serial.println("Alert: Object detected within 10cm range.");
        Serial.println("Sending SMS Alert..."); //Kinoment ni kent kasi panggulo
        //Binago ni kent debug para sa cm
        String message = "Alert: Object detected within 10cm range. "
                         "Sensor 1: " + String(distance1) + " cm, "
                         "Sensor 2: " + String(distance2) + " cm, "
                         "Sensor 3: " + String(distance3) + " cm.";
        sendSMS(message);
        delay(10000); // Avoid spamming messages
    } else {
        Serial.println("No SMS Alert Sent");
        Serial.println("No object detected.");
    }

    delay(1000);
}

// Function to get distance using ultrasonic sensor
int getDistance(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2;
    return distance;
}

// Function to send SMS alert via GSM module
void sendSMS(String message) {
    sim.println("AT+CMGF=1");  // Set SMS mode to text
    delay(500);
    sim.print("AT+CMGS=\"");
    sim.print(phoneNumber);
    sim.println("\"");
    delay(500);
    sim.println(message);
    delay(500);
    sim.write(26); // Send Ctrl+Z to send the message
    delay(500);
}
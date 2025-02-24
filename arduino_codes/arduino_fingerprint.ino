#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define SERVO_PIN 4  
#define FP_RX 2  // Connect to fingerprint sensor TX
#define FP_TX 3  // Connect to fingerprint sensor RX

SoftwareSerial fingerSerial(FP_RX, FP_TX); // SoftwareSerial for fingerprint sensor
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerSerial);
Servo myServo;

void setup() {
  myServo.attach(SERVO_PIN);
  myServo.write(0); // Servo in closed position

  Serial.begin(9600); // Communicate with ESP32
  fingerSerial.begin(57600); // Fingerprint sensor communication

  delay(1000);

  if (finger.verifyPassword()) {
    Serial.println("READY");
  } else {
    Serial.println("ERROR: Sensor Not Found");
    while (1);
  }
}

void loop() {
  if (Serial.available()) { 
    char command = Serial.read(); // Read data from ESP32
    if (command == 'E' || command == 'e') {
      Serial.println("ENROLLING"); // Send status to ESP32
      enrollFingerprint();
    }
  }
  checkFingerprintMatch();
}

// Function to enroll a fingerprint in two steps
void enrollFingerprint() {
  Serial.println("PLACE FINGER"); // First scan
  if (!captureFingerprint(1)) return;
  
  Serial.println("REMOVE FINGER"); // Wait for user to remove finger
  delay(2000);
  while (finger.getImage() != FINGERPRINT_NOFINGER); 

  Serial.println("PLACE FINGER AGAIN"); // Second scan
  if (!captureFingerprint(2)) return;

  if (finger.createModel() != FINGERPRINT_OK) {
    Serial.println("ERROR: Fingerprints did not match");
    return;
  }

  if (finger.storeModel(1) == FINGERPRINT_OK) {
    Serial.println("SUCCESS: Fingerprint stored");
    delay(2000);
  } else {
    Serial.println("ERROR: Storing failed");
  }
}

// Function to capture fingerprint image
bool captureFingerprint(uint8_t bufferID) {
  int p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }
  return (finger.image2Tz(bufferID) == FINGERPRINT_OK);
}

// Function to check for fingerprint match
void checkFingerprintMatch() {
  if (finger.getImage() != FINGERPRINT_OK) return;
  if (finger.image2Tz(1) != FINGERPRINT_OK) return;

  int p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("MATCH/Door Opened");
    activateServo();
  } else {
    Serial.println("NO MATCH/Door Closed");
  }
}

// Function to control the servo motor
void activateServo() {
  myServo.write(90);
  delay(5000);
  myServo.write(0);
}
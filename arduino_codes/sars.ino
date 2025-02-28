#include <SoftwareSerial.h> // Necessary for module communication 
 
SoftwareSerial sim(10, 11); // Marking the pins as the pins that will be use for module communication	 
int trigPin = 8; //Ultrasonic trigger pin 
int echoPin = 9; //Ultrasonic echo pin 
long duration;  
int distance; //Calculated distance 
int _timeout; 
String _buffer;  

String number = "+639763709707"; //Change to recipient’s phone number 

 

void setup() {
  
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); // Receives input waves from the trigger  
  Serial.begin(9600); 
  //Serial1.begin(9600);
  _buffer.reserve(50); 
  Serial.println("..."); 
  sim.begin(9600); 
  delay(5000); 

} 

 

void loop() { 

  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 

  
  duration = pulseIn(echoPin, HIGH); 
  distance = duration * 0.034 / 2;

 


 if (distance > 0 && distance < 400) { 

  Serial.print("Distance: "); 
  Serial.print(distance); 
  Serial.println(" cm"); 

 

if (distance <= 10) { 

    SendMessage(); 
      delay(5000);// Add a delay to avoid repeated triggers within a short time 
    } 

  }else { 
    Serial.println("out of range");
    Serial.println("standby to send message");
    //delay(10000); // para iwas bluefcking screen lol
  } 


if (sim.available() > 0) { 

  Serial.write(sim.read()); 

 } 

} 

 

void SendMessage() { 

  sim.println("AT+CMGF=1"); 
  delay(200); 
  sim.println("AT+CMGS=\"" + number + "\"\r");  
  delay(200); 
  String SMS = "Alert: Object detected!!"; //Message For recipient
  Serial.println("Alert: Sensor trigger sending Message. Sending message to: " + number);
  Serial.println("Sensor trigger sending Message!");
  //sim.println(SMS); 
  delay(100); 
  sim.println((char)26);  
  delay(200); 
  _buffer = _readSerial();
  //Serial.println(number);
  //Serial.println(SMS);
} 

 

String _readSerial() { 
  
  _timeout = 0; 
  while (!sim.available() && _timeout < 12000) { 
  delay(13); 
  _timeout++; 

  } 
    if (sim.available()) { 
    return sim.readString(); 
  } 
    return ""; 
}
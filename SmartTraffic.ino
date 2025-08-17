#define BLYNK_TEMPLATE_NAME "traficlight"
#define BLYNK_AUTH_TOKEN "cFACbUqqwP0tgy_fYzocXmQG2ocOZ3N3"
#define BLYNK_TEMPLATE_ID "TMPL3vMnhlslI"
#include <Arduino.h>
#include <NewPing.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Define pins for Traffic Lights
const int redLight1 = 21;
const int yellowLight1 = 19;
const int greenLight1 = 18;

const int redLight2 = 0;
const int yellowLight2 = 4;
const int greenLight2 = 16;

const int redLight3 = 25;
const int yellowLight3 = 33;
const int greenLight3 = 32;

const int redLight4 = 14;
const int yellowLight4 = 27;
const int greenLight4 = 26;

// Define pins for Ultrasonic Sensors
const int trigPin1 = 23;
const int echoPin1 = 22;

const int trigPin2 = 15;
const int echoPin2 = 2;

const int trigPin3 = 34;
const int echoPin3 = 36;

const int trigPin4 = 13;
const int echoPin4 = 12;

// Define maximum distance for sensor
#define MAX_DISTANCE 400

NewPing sensor1(trigPin1, echoPin1, MAX_DISTANCE);
NewPing sensor2(trigPin2, echoPin2, MAX_DISTANCE);
NewPing sensor3(trigPin3, echoPin3, MAX_DISTANCE);
NewPing sensor4(trigPin4, echoPin4, MAX_DISTANCE);

// Blynk authentication token
char auth[] = "cFACbUqqwP0tgy_fYzocXmQG2ocOZ3N3";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

void setup() {
  Serial.begin(115200);
  
  // Initialize traffic light pins as outputs
  pinMode(redLight1, OUTPUT);
  pinMode(yellowLight1, OUTPUT);
  pinMode(greenLight1, OUTPUT);

  pinMode(redLight2, OUTPUT);
  pinMode(yellowLight2, OUTPUT);
  pinMode(greenLight2, OUTPUT);

  pinMode(redLight3, OUTPUT);
  pinMode(yellowLight3, OUTPUT);
  pinMode(greenLight3, OUTPUT);

  pinMode(redLight4, OUTPUT);
  pinMode(yellowLight4, OUTPUT);
  pinMode(greenLight4, OUTPUT);

  // Turn off all lights initially
  allLightsOff();

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  
  // Read distance from sensors
  int distance1 = sensor1.ping_cm();
  int distance2 = sensor2.ping_cm();
  int distance3 = sensor3.ping_cm();
  int distance4 = sensor4.ping_cm();

  // Print sensor values to the Serial Monitor (for debugging)
  Serial.print("Distance1: "); Serial.print(distance1); Serial.print(" cm, ");
  Serial.print("Distance2: "); Serial.print(distance2); Serial.print(" cm, ");
  Serial.print("Distance3: "); Serial.print(distance3); Serial.print(" cm, ");
  Serial.print("Distance4: "); Serial.print(distance4); Serial.println(" cm");

  // Send sensor values to Blynk
  Blynk.virtualWrite(V13, distance1);
  Blynk.virtualWrite(V15, distance2);
  Blynk.virtualWrite(V34, distance3);
  Blynk.virtualWrite(V23, distance4);

  // Implement your logic here based on the sensor values
  controlTrafficLights(distance1, distance2, distance3, distance4);

  // Wait before repeating the loop
  delay(1000);
}

void controlTrafficLights(int d1, int d2, int d3, int d4) {
  // Example logic for controlling traffic lights based on sensor distance
  // This is a very basic example, you can enhance it further as needed

  // Reset all lights
  allLightsOff();

  // Simple logic to turn on green light if distance is less than threshold
  if (d1 > 0 && d1 < 20) {
    digitalWrite(greenLight1, HIGH);
  } else {
    digitalWrite(redLight1, HIGH);
  }

  if (d2 > 0 && d2 < 20) {
    digitalWrite(greenLight2, HIGH);
  } else {
    digitalWrite(redLight2, HIGH);
  }

  if (d3 > 0 && d3 < 20) {
    digitalWrite(greenLight3, HIGH);
  } else {
    digitalWrite(redLight3, HIGH);
  }

  if (d4 > 0 && d4 < 20) {
    digitalWrite(greenLight4, HIGH);
  } else {
    digitalWrite(redLight4, HIGH);
  }
}

void allLightsOff() {
  digitalWrite(redLight1,LOW);
  digitalWrite(yellowLight1,LOW);
  digitalWrite(greenLight1,LOW);

  digitalWrite(redLight2,LOW);
  digitalWrite(yellowLight2,LOW);
  digitalWrite(greenLight2,LOW);

  digitalWrite(redLight3,LOW);
  digitalWrite(yellowLight3,LOW);
  digitalWrite(greenLight3,LOW);

  digitalWrite(redLight4,LOW);
  digitalWrite(yellowLight4,LOW);
  digitalWrite(greenLight4,LOW);
}

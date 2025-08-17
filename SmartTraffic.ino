// Smart Traffic Light Control using ESP32 + Ultrasonic Sensors + Blynk
// Idea: Only one road can have green at a time, others stay red
// Priority is given to the road with more vehicles (shortest distance)

#define BLYNK_TEMPLATE_NAME "traficlight"
#define BLYNK_AUTH_TOKEN "cFACbUqqwP0tgy_fYzocXmQG2ocOZ3N3"
#define BLYNK_TEMPLATE_ID "TMPL3vMnhlslI"

#include <Arduino.h>
#include <NewPing.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Traffic lights – 4 roads, each has Red, Yellow, Green
int redLight1 = 21, yellowLight1 = 19, greenLight1 = 18;
int redLight2 = 0,  yellowLight2 = 4,  greenLight2 = 16;
int redLight3 = 25, yellowLight3 = 33, greenLight3 = 32;
int redLight4 = 14, yellowLight4 = 27, greenLight4 = 26;

// Ultrasonic sensors for each road
int trigPin1 = 23, echoPin1 = 22;
int trigPin2 = 15, echoPin2 = 2;
int trigPin3 = 34, echoPin3 = 36;
int trigPin4 = 13, echoPin4 = 12;

#define MAX_DISTANCE 400
NewPing sensor1(trigPin1, echoPin1, MAX_DISTANCE);
NewPing sensor2(trigPin2, echoPin2, MAX_DISTANCE);
NewPing sensor3(trigPin3, echoPin3, MAX_DISTANCE);
NewPing sensor4(trigPin4, echoPin4, MAX_DISTANCE);

// Blynk + WiFi
char auth[] = "cFACbUqqwP0tgy_fYzocXmQG2ocOZ3N3";
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

void setup() {
  Serial.begin(115200);

  // set all LEDs as outputs
  pinMode(redLight1, OUTPUT); pinMode(yellowLight1, OUTPUT); pinMode(greenLight1, OUTPUT);
  pinMode(redLight2, OUTPUT); pinMode(yellowLight2, OUTPUT); pinMode(greenLight2, OUTPUT);
  pinMode(redLight3, OUTPUT); pinMode(yellowLight3, OUTPUT); pinMode(greenLight3, OUTPUT);
  pinMode(redLight4, OUTPUT); pinMode(yellowLight4, OUTPUT); pinMode(greenLight4, OUTPUT);

  // start with all lights off
  allLightsOff();

  // connect to WiFi + Blynk
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();

  // take readings from all sensors
  int d1 = sensor1.ping_cm();
  int d2 = sensor2.ping_cm();
  int d3 = sensor3.ping_cm();
  int d4 = sensor4.ping_cm();

  // print values to serial (debugging)
  Serial.print("Road1: "); Serial.print(d1);
  Serial.print(" cm | Road2: "); Serial.print(d2);
  Serial.print(" cm | Road3: "); Serial.print(d3);
  Serial.print(" cm | Road4: "); Serial.println(d4);

  // send data to Blynk dashboard
  Blynk.virtualWrite(V13, d1);
  Blynk.virtualWrite(V15, d2);
  Blynk.virtualWrite(V34, d3);
  Blynk.virtualWrite(V23, d4);

  // decide which road gets green
  controlTrafficLights(d1, d2, d3, d4);

  delay(2000); // wait before next check
}

// logic to decide traffic lights
void controlTrafficLights(int d1, int d2, int d3, int d4) {
  allLightsOff();

  int minDist = 999;
  int road = 0;

  // pick the road with shortest distance (more traffic closer)
  if (d1 > 0 && d1 < minDist) { minDist = d1; road = 1; }
  if (d2 > 0 && d2 < minDist) { minDist = d2; road = 2; }
  if (d3 > 0 && d3 < minDist) { minDist = d3; road = 3; }
  if (d4 > 0 && d4 < minDist) { minDist = d4; road = 4; }

  // if no vehicles anywhere, just keep road 1 green
  if (road == 0) road = 1;

  // now actually turn on lights
  if (road == 1) { digitalWrite(greenLight1, HIGH); digitalWrite(redLight2, HIGH); digitalWrite(redLight3, HIGH); digitalWrite(redLight4, HIGH); }
  if (road == 2) { digitalWrite(greenLight2, HIGH); digitalWrite(redLight1, HIGH); digitalWrite(redLight3, HIGH); digitalWrite(redLight4, HIGH); }
  if (road == 3) { digitalWrite(greenLight3, HIGH); digitalWrite(redLight1, HIGH); digitalWrite(redLight2, HIGH); digitalWrite(redLight4, HIGH); }
  if (road == 4) { digitalWrite(greenLight4, HIGH); digitalWrite(redLight1, HIGH); digitalWrite(redLight2, HIGH); digitalWrite(redLight3, HIGH); }

  Serial.print("Green given to Road "); Serial.println(road);
}

// helper function to reset lights
void allLightsOff() {
  
  digitalWrite(redLight1, LOW); digitalWrite(yellowLight1, LOW); digitalWrite(greenLight1, LOW);
  
  digitalWrite(redLight2, LOW); digitalWrite(yellowLight2, LOW); digitalWrite(greenLight2, LOW);
  
  digitalWrite(redLight3, LOW); digitalWrite(yellowLight3, LOW); digitalWrite(greenLight3, LOW);
  
  digitalWrite(redLight4, LOW); digitalWrite(yellowLight4, LOW); digitalWrite(greenLight4, LOW);

}

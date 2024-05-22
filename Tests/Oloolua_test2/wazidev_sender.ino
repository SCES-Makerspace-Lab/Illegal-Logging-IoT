#include <SPI.h>
#include <LoRa.h>

#define ECHO_PIN 4
#define TRIG_PIN 5
#define SS_PIN 10 // You can choose any available digital pin for SS

void setup() {
  Serial.begin(38400);
  while (!Serial);

  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);

  // LoRa initialization
  SPI.begin();
  LoRa.setPins(SS_PIN, 19, 27); // SS, RST, DI0
  if (!LoRa.begin(868E6)) { // Change frequency to 868 MHz
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

float distance_centimeter() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  float dist = distance_centimeter();
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");

  LoRa.beginPacket();
  LoRa.print(dist);
  LoRa.endPacket();

  delay(1000);
}

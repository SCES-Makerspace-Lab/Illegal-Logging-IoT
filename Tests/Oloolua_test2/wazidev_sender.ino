#include <SPI.h>
#include <LoRa.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define SS_PIN 10 // You can choose any available digital pin for SS

SoftwareSerial mySerial(9, 11); // Connect GPS TX to pin 9, GPS RX to pin 11
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  mySerial.begin(19200); // Start the software serial port for GPS
  Serial.println("GPS Receiver Initialized");
  delay(1000); // Delay to ensure everything is powered up

  // LoRa initialization
  SPI.begin();
  LoRa.setPins(SS_PIN, 19, 27); // SS, RST, DI0
  if (!LoRa.begin(866E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (mySerial.available() > 0) {
    if (gps.encode(mySerial.read())) {
      if (gps.location.isValid() && gps.time.isValid()) {
        float lat = gps.location.lat();
        float lng = gps.location.lng();
        int hour = (gps.time.hour() + 3) % 24; // Adjust time for Nairobi (UTC+3)
        int minute = gps.time.minute();
        int second = gps.time.second();

        Serial.print("Latitude: ");
        Serial.println(lat, 6);
        Serial.print("Longitude: ");
        Serial.println(lng, 6);
        Serial.print("Time (Nairobi): ");
        Serial.print(hour);
        Serial.print(":");
        Serial.print(minute);
        Serial.print(":");
        Serial.println(second);

        LoRa.beginPacket();
        LoRa.print(lat);
        LoRa.print(",");
        LoRa.print(lng);
        LoRa.print(",");
        LoRa.print(hour);
        LoRa.print(":");
        LoRa.print(minute);
        LoRa.print(":");
        LoRa.print(second);
        LoRa.endPacket();
      }
    }
  }
}
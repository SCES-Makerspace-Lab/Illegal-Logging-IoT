#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10 // You can choose any available digital pin for SS
#define LCD_ADDRESS 0x27 // I2C address of your LCD
#define MAX_WAIT_TIME 10000 // Maximum wait time for LoRa packet in milliseconds

LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2); // Define LCD

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Lat: ");
  lcd.setCursor(0, 1);
  lcd.print("Lng: ");
  lcd.setCursor(0, 2);
  lcd.print("Time: ");

  // LoRa initialization
  SPI.begin();
  LoRa.setPins(SS_PIN, 19, 27); // SS, RST, DI0
  if (!LoRa.begin(866E6)) { // Change frequency to 866 MHz
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // Listen for LoRa packets
  unsigned long startTime = millis();
  int packetSize;
  while ((packetSize = LoRa.parsePacket()) == 0) {
    if (millis() - startTime > MAX_WAIT_TIME) {
      Serial.println("No LoRa packet received, receiver out of range!");
      lcd.setCursor(0, 1);
      lcd.print("Receiver out of");
      lcd.setCursor(0, 2);
      lcd.print("range!        ");
      break;
    }
  }
  
  if (packetSize) {
    while (LoRa.available()) {
      String receivedData = LoRa.readString();
      int firstCommaIndex = receivedData.indexOf(',');
      int secondCommaIndex = receivedData.indexOf(',', firstCommaIndex + 1);
      float lat = receivedData.substring(0, firstCommaIndex).toFloat();
      float lng = receivedData.substring(firstCommaIndex + 1, secondCommaIndex).toFloat();
      String time = receivedData.substring(secondCommaIndex + 1);
      int rssi = LoRa.packetRssi(); // Get RSSI value
      float snr = LoRa.packetSnr(); // Get SNR value

      Serial.print("Received Latitude: ");
      Serial.print(lat);
      Serial.print(", Longitude: ");
      Serial.print(lng);
      Serial.print(", Time (Nairobi): ");
      Serial.print(time);
      Serial.print(", RSSI: ");
      Serial.print(rssi);
      Serial.print(" dBm, SNR: ");
      Serial.print(snr);
      Serial.println(" dB");

      // Display latitude, longitude, and time on LCD
      lcd.setCursor(5, 0);
      lcd.print("     "); // Clear previous latitude value
      lcd.setCursor(5, 0);
      lcd.print(lat);

      lcd.setCursor(5, 1);
      lcd.print("     "); // Clear previous longitude value
      lcd.setCursor(5, 1);
      lcd.print(lng);

      lcd.setCursor(6, 2);
      lcd.print("        "); // Clear previous time value
      lcd.setCursor(6, 2);
      lcd.print(time);

      delay(1000); // Delay between display updates
    }
  }
}
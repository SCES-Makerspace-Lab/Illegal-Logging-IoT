#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD with I2C address 0x27 (adjust if necessary)
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SS_PIN 10 // You can choose any available digital pin for SS

void setup() {
  Serial.begin(38400);
  while (!Serial);

  // Initialize the LCD
  lcd.begin(16, 2); // specify the LCD's number of columns and rows
  lcd.backlight();

  // LoRa initialization
  SPI.begin();
  LoRa.setPins(SS_PIN, 19, 27); // SS, RST, DI0
  if (!LoRa.begin(868E6)) { // Make sure the frequency matches the transmitter
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // Try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Read the packet
    String distance = "";
    while (LoRa.available()) {
      distance += (char)LoRa.read();
    }

    // Get packet SNR and RSSI
    int packetSNR = LoRa.packetSnr();
    int packetRSSI = LoRa.packetRssi();

    // Print data to Serial
    Serial.print("Received: ");
    Serial.println(distance);
    Serial.print("Packet SNR: ");
    Serial.println(packetSNR);
    Serial.print("Packet RSSI: ");
    Serial.println(packetRSSI);

    // Display data on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dist: ");
    lcd.print(distance);
    lcd.print(" cm");

    lcd.setCursor(0, 1);
    lcd.print("SNR: ");
    lcd.print(packetSNR);
    lcd.print(" RSSI: ");
    lcd.print(packetRSSI);
  }
}
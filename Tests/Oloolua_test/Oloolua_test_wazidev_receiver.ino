
#include <Wire.h>
#include <LoRa.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27 // I2C address of your LCD
#define LCD_COLUMNS 16 // Number of columns in LCD
#define LCD_ROWS 2 // Number of rows in LCD
#define MAX_WAIT_TIME 10000 // Maximum wait time for LoRa packet in milliseconds

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize LCD display
  lcd.init();
  lcd.backlight(); // Turn on backlight
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for LoRa");

  // LoRa initialization
  SPI.begin();
  if (!LoRa.begin(866E6)) { // Change frequency to 866 MHz
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa initialized successfully.");
}

void loop() {
  // Listen for LoRa packets
  unsigned long startTime = millis();
  int packetSize;
  while ((packetSize = LoRa.parsePacket()) == 0) {
    if (millis() - startTime > MAX_WAIT_TIME) {
      Serial.println("No LoRa packet received, receiver out of range!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Receiver out of");
      lcd.setCursor(0, 1);
      lcd.print("range!");
      break;
    }
  }

  if (packetSize) {
    float receivedIntensity;
    float receivedSNR;
    float receivedRSSI;
    float receivedDistance;

    // Only print formatted data once for each received packet
    if (LoRa.available()) {
      byte data[16];
      LoRa.readBytes(data, 16);

      // Convert bytes back to float data
      receivedIntensity = bytesToFloat(&data[0]);
      receivedSNR = bytesToFloat(&data[4]);
      receivedRSSI = bytesToFloat(&data[8]);
      receivedDistance = bytesToFloat(&data[12]);

      // Print formatted data to Serial
      Serial.print("Received Intensity (dB): ");
      Serial.println(receivedIntensity);
      Serial.print("Received SNR (dB): ");
      Serial.println(receivedSNR);
      Serial.print("Received RSSI (dBm): ");
      Serial.println(receivedRSSI);
      Serial.print("Received Distance (cm): ");
      Serial.println(receivedDistance);

      // Display data on LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SNR: ");
      lcd.print(receivedSNR);
      lcd.print(" dB  ");
      lcd.setCursor(0, 1);
      lcd.print("RSSI: ");
      lcd.print(receivedRSSI);
      lcd.print(" dBm");
    }

    delay(5000); // Delay between display updates
  }
}

// Convert bytes to float
float bytesToFloat(byte* bytes_array) {
  union {
    float float_variable;
    byte temp_array[4];
  } u;
  memcpy(u.temp_array, bytes_array, 4);
  return u.float_variable;
}
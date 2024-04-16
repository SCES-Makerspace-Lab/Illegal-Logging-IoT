#include <Wire.h>
#include <LoRa.h>

#define SS_PIN 10 // You can choose any available digital pin for SS

// Convert bytes to float
float bytesToFloat(byte* bytes_array) {
  union {
    float float_variable;
    byte temp_array[4];
  } u;
  memcpy(u.temp_array, bytes_array, 4);
  return u.float_variable;
}

void setup() {
  Wire.begin(8); // Start I2C communication with address 8
  Wire.onReceive(receiveData); // Register callback function for receiving data
  Serial.begin(9600); // Initialize serial communication

  // LoRa initialization
  SPI.begin();
  LoRa.setPins(SS_PIN, 19, 27); // SS, RST, DI0
  if (!LoRa.begin(866E6)) { // Change frequency to 866 MHz
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // Add any additional code here if needed
}

void receiveData(int byteCount) {
  byte data[4];

  if (Wire.available() >= 16) {
    Wire.readBytes(data, 4);
    float intensity = bytesToFloat(data);

    Wire.readBytes(data, 4);
    float snr = bytesToFloat(data);

    Wire.readBytes(data, 4);
    float rssi = bytesToFloat(data);

    Wire.readBytes(data, 4);
    float distance = bytesToFloat(data);

    Serial.print("Received Intensity (dB): ");
    Serial.println(intensity);
    Serial.print("Received SNR (dB): ");
    Serial.println(snr);
    Serial.print("Received RSSI (dBm): ");
    Serial.println(rssi);
    Serial.print("Received Estimated Distance (cm): ");
    Serial.println(distance);
    Serial.println(); // Print an empty line for clarity

    // Transmit data via LoRa
    sendLoRaData(intensity, snr, rssi, distance);

    delay(1000); // Wait for 1 second before processing the next set of data
  }
}

void sendLoRaData(float intensity, float snr, float rssi, float distance) {
  byte data[16];

  // Convert float data to bytes
  floatToBytes(intensity, &data[0]);
  floatToBytes(snr, &data[4]);
  floatToBytes(rssi, &data[8]);
  floatToBytes(distance, &data[12]);

  // Send data via LoRa
  LoRa.beginPacket();
  LoRa.write(data, 16);
  LoRa.endPacket();
}

// Convert float to bytes
void floatToBytes(float val, byte* bytes_array) {
  union {
    float float_variable;
    byte temp_array[4];
  } u;
  u.float_variable = val;
  memcpy(bytes_array, u.temp_array, 4);
}
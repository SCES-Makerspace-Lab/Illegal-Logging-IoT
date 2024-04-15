#include <Wire.h>

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

    delay(1000); // Wait for 1 second before processing the next set of data
  }
}
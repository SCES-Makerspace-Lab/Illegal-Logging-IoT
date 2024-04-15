#include <PDM.h>
#include <Wire.h>

// Buffer to read samples into, each sample is 16-bits
short sampleBuffer[256];

// Number of samples read
volatile int samplesRead;

// Microphone specifications
const float microphoneSensitivity = -26.0; // dBFS ±3 dB
const float microphoneSNR = 64.0; // dB
const float microphoneAOP = 122.5; // dBSPL

// Convert float to bytes
void floatToBytes(float val, byte* bytes_array) {
  union {
    float float_variable;
    byte temp_array[4];
  } u;
  u.float_variable = val;
  memcpy(bytes_array, u.temp_array, 4);
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Configure the data receive callback
  PDM.onReceive(onPDMdata);

  // Initialize PDM with:
  // - one channel (mono mode)
  // - a 16 kHz sample rate
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }

  // Initialize I2C communication
  Wire.begin();
}

void loop() {
  // Wait for samples to be read
  if (samplesRead) {

    // Print samples to the serial monitor
    for (int i = 0; i < samplesRead; i++) {
      // Calculate sound intensity (dB)
      float intensity = 20 * log10(abs(sampleBuffer[i]));

      // Calculate SNR (Signal-to-Noise Ratio)
      float snr = intensity - microphoneSNR;

      // Calculate RSSI (Received Signal Strength Indicator)
      float rssi = intensity - microphoneSensitivity;

      // Calculate Estimated Distance
      // Note: This is a simplistic approach and may not accurately reflect real-world distances
      float distance = 100 / (intensity + 0.001); // Adding a small value to prevent division by zero
      // Output results
      delay(1000);
      Serial.print("Sound Intensity (dB): ");
      Serial.println(intensity, 2);
      Serial.print("SNR (dB): ");
      Serial.println(snr, 2);
      Serial.print("RSSI (dBm): ");
      Serial.println(rssi, 2);
      Serial.print("Estimated Distance (cm): ");
      Serial.println(distance, 2);
      Serial.println(); // Print an empty line for clarity
      delay(1000);

      // Send data to WaziDev board
      byte data[4];
      Wire.beginTransmission(8); // I2C address of the WaziDev board

      floatToBytes(intensity, data);
      Wire.write(data, 4);

      floatToBytes(snr, data);
      Wire.write(data, 4);

      floatToBytes(rssi, data);
      Wire.write(data, 4);

      floatToBytes(distance, data);
      Wire.write(data, 4);

      Wire.endTransmission(); // End transmission
    }

    // Introduce a delay between measurements
    delay(1000); // Adjust delay as needed

    // Clear the read count
    samplesRead = 0;
  }
}

void onPDMdata() {
  // Query the number of bytes available
  int bytesAvailable = PDM.available();

  // Read into the sample buffer
  PDM.read(sampleBuffer, bytesAvailable);

  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;
}
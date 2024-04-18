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

// Assume a constant noise floor value (adjust this with real measurements if possible)
const float estimatedNoiseFloor = -50.0; // dB

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
      float intensity = 20 * log10(abs(sampleBuffer[i]) / 32768.0); // Normalize to the maximum possible value (16-bit signed integer)

      // Calculate SNR (Signal-to-Noise Ratio)
      float snr = intensity - estimatedNoiseFloor;

      // Calculate RSSI (Received Signal Strength Indicator)
      // Adjusted to compare against full scale (0 dBFS)
      float rssi = intensity - (-26.0); // Maximum value from microphone sensitivity

      // Calculate Estimated Distance
      float distance = 100 / (intensity + 0.001); // Adding a small value to prevent division by zero

      // Output results
      Serial.print("Sound Intensity (dB): ");
      Serial.println(intensity, 2);
      Serial.print("SNR (dB): ");
      Serial.println(snr, 2);
      Serial.print("RSSI (dB): ");
      Serial.println(rssi, 2);
      Serial.print("Estimated Distance (cm): ");
      Serial.println(distance, 2);
      Serial.println(); // Print an empty line for clarity

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

# LoRa Range Test with GPS on WaziDev

This project is designed to perform a range test for LoRa using the WaziDev board. The sender WaziDev board collects GPS coordinates and sends them to the receiver WaziDev board.

## Components

- 2x WaziDev boards
- 1x GPS module

## How it Works

The sender WaziDev board is connected to a GPS module. It collects the GPS coordinates (latitude and longitude) and the current time in Nairobi. This data is then sent over LoRa to the receiver WaziDev board.

The receiver WaziDev board listens for incoming LoRa packets. When a packet is received, it extracts the GPS coordinates and time from the packet. The received data is displayed on the Serial Monitor and an LCD.

In addition to the GPS data, the receiver also displays the RSSI (Received Signal Strength Indicator) and SNR (Signal-to-Noise Ratio) of the LoRa connection. These values provide an indication of the quality of the LoRa signal, which can be used to assess the range of the LoRa connection.

## Setup

1. Connect the GPS module to the sender WaziDev board. Connect the GPS TX pin to digital pin 9 and the GPS RX pin to digital pin 11.

2. Upload the sender code to the sender WaziDev board.

3. Connect an LCD to the receiver WaziDev board. The LCD is connected via I2C, so connect the SDA pin to the SDA pin on the WaziDev and the SCL pin to the SCL pin on the WaziDev.

4. Upload the receiver code to the receiver WaziDev board.

5. Open the Serial Monitor for both WaziDev boards. The sender should display the GPS coordinates and time, and the receiver should display the received GPS coordinates, time, RSSI, and SNR.

## Note

Ensure the GPS module is receiving a good signal. If the GPS module is indoors or in a location where it can't get a good signal, it may not be able to provide valid location data.
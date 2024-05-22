import serial
import time
from datetime import datetime

# Open the serial port
ser = serial.Serial('/dev/ttyUSB1', 19200)

# Open the file to save the data
with open('/home/sheikh/Desktop/data.txt', 'a') as file:
    # Write the table header
    file.write('Timestamp\t\t\tReceived\tSNR\tRSSI\n')

    received = snr = rssi = ''
    while True:
        # Read a line from the serial port
        line = ser.readline().decode('utf-8').strip()

        # Check if the line contains the expected data
        if "Received:" in line:
            received = line.split(":")[1].strip()
        elif "Packet SNR:" in line:
            snr = line.split(":")[1].strip()
        elif "Packet RSSI:" in line:
            rssi = line.split(":")[1].strip()

            # Get the current timestamp
            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

            # Write the timestamp and data to the file
            file.write(f'{timestamp}\t{received}\t{snr}\t{rssi}\n')

            # Print the timestamp and data
            print(f'{timestamp}\t{received}\t{snr}\t{rssi}')

            # Reset the values
            received = snr = rssi = ''
        elif "Connection lost!" in line:
            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            file.write(f'{timestamp}\tConnection lost!\n')
            print(f'{timestamp}\tConnection lost!')
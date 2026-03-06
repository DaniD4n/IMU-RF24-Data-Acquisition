# Wireless IMU Data Acquisition (nRF24 + Python)

This project implements a wireless IMU acquisition system using:

- Arduino + nRF24L01
- Binary custom protocol
- Python real-time visualization with Matplotlib

## Features

- Header-based packet synchronization (0xAA 0x55)
- 16-byte packed struct transmission
- Circular buffers for stable plotting
- Real-time accelerometer and gyroscope visualization

## Packet Structure

| Field      | Type     | Bytes |
|------------|----------|-------|
| Timestamp  | uint32   | 4     |
| ax, ay, az | int16 x3 | 6     |
| gx, gy, gz | int16 x3 | 6     |

Total: 16 bytes (+ 2-byte header)

## Installation

pip install -r python/requirements.txt

## Run

python python/main.py


## Future Improvements

- Checksum validation
- CSV logging
- FFT analysis
- Sensor calibration

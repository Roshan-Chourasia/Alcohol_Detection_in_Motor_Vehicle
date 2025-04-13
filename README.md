Alcohol Detection in Motor Vehicle 🚗🍻🚨
------------------------------------------
📌 Project Overview:
This project focuses on enhancing road safety by implementing an IoT-based Alcohol Detection and Accident Prevention System. The system utilizes an MQ-3 alcohol sensor, Arduino Uno, SIM800L GSM module, and Ublox Neo 6M GPS module to detect alcohol levels in real-time, control the motor (representing the vehicle engine), and automatically send SMS alerts with the current GPS location if alcohol is detected.

💡 Features:
🚗 Real-Time Alcohol Detection
📡 SMS Alerts via GSM Module
📍 Location Tracking with GPS Module
🔒 Motor Lock Mechanism for Accident Prevention
📟 16x2 LCD Status Display
💾 Modular, Expandable and Embedded-Friendly Codebase

⚙️ Technology Stack:
Arduino Uno
MQ-3 Alcohol Sensor
SIM800L GSM Module
Ublox Neo 6M GPS Module
L298N Motor Driver
Embedded C/C++
IoT & Embedded Systems

🏗️ Working Principle:
1. The MQ-3 sensor continuously samples the surrounding air to detect alcohol concentration.
2. If the measured value exceeds a preset threshold, the system:
   - Sends an SMS alert with the GPS location to a predefined mobile number.
   - Displays "ALCOHOL DETECTED" on the LCD.
   - Prevents the motor from starting (simulating engine lock).

3. If no alcohol is detected:
   - The motor operates normally.
   - The LCD displays "NO ALCOHOL DETECTED".

🧑‍💻 Code Overview
alcohol_detection.ino - Main Arduino sketch for sensor reading, motor control, SMS handling, and GPS data processing.

Libraries Used:
- SoftwareSerial.h
- TinyGPS++.h
- LiquidCrystal_I2C.h

🎥 Demo Video: https://drive.google.com/file/d/1xmOVn6fi_zZaXeTfBhdUjA0eh6mxeaIm/view?usp=sharing

💬 Conclusion:
This project demonstrates how embedded systems and IoT can be integrated to address real-world road safety problems. By ensuring alcohol detection before a vehicle can start and sending automated alerts, this system aims to reduce drink-and-drive incidents.

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

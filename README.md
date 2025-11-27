# Development of robotic arm and conveyor for colour-based sorting operation
Pick‑and‑place robotic arm with a motorized conveyor that sorts small objects into bins based on detected color using an Arduino controlled GY‑31 (TCS3200) color sensor and IR object detection.

# Overview
This project implements an automated color‑based sorting system using a 2‑DOF acrylic robotic arm and a wooden conveyor belt. Objects move along the conveyor, are detected by an IR sensor, classified by a GY‑31 color sensor and then picked and placed by the arm into designated color bins.
The prototype was developed as a Bachelor thesis project in Instrumentation and Control Engineering and later donated to the college laboratory as a demonstration model for industrial automation concepts.

# Features
- Automated pick‑and‑place sorting of objects into bins based on detected RGB color.
- Integrated conveyor stop/start using IR object detection to position items precisely under the color sensor.
- 2‑DOF robotic arm with custom acrylic gripper driven by MG995 and SG90 servo motors.
- GY‑31 (TCS3200) color sensor with configurable photodiode selection and frequency scaling for RGB measurement.
- Relay‑controlled 12 V DC conveyor motor with adjustable speed using potentiometer.
- Status indication using color‑matched LEDs on the placement panel during sorting.

# System Architecture
1. Object placed on conveyor and transported toward the sensing area.  
2. IR sensor detects object presence and signals the Arduino to stop the conveyor motor via a relay.  
3. GY‑31 color sensor measures the object’s RGB values and classifies the color using threshold logic.  
4. Arduino actuates the robotic arm:  
   - Rotate base servo to pickup position.  
   - Close gripper servo to grasp object.  
   - Rotate to the target color bin position.  
   - Open gripper to release and return to home.  
5. Conveyor restarts and the cycle repeats.  

# Hardware and components used
- Arduino Uno
- 2‑axis acrylic robotic arm (2 DOF) with custom acrylic gripper
- Servo motors:  
  - MG995 (arm joints, high‑torque positioning)
  - SG90 (gripper open/close)
- Conveyor:  
  - Wooden frame (approx. 65 × 8 × 10 cm)
  - Fabric belt driven by 12 V DC motor with speed control using potentiometer
- Sensors:  
  - GY‑31 (TCS3200) color sensor module for RGB detection
  - IR sensor modules for object presence detection
- Power and switching:  
  - 5 V / 5 A SMPS for electronics and servo supply
  - Relay module for switching the 12 V conveyor motor

# Applications and Limitations
Application domains include:
- Educational demonstration of industrial material handling and sorting.  
- Packaging and manufacturing lines for small colored components.  
- Introductory platform for experimenting with color sensors and basic robotics.  
Known limitations:  
- Performance is sensitive to ambient lighting and requires calibration of colors.
- Limited number of discrete colors supported and manual tuning needed for new objects.
- Mechanical design and gripping are optimized for simple, regular shaped objects.

# How to Use This Repository
- `/src/` – Arduino sketch for the robotic arm, conveyor, color sensor and IR sensor integration  
- `/hardware/` – Schematics, wiring diagrams and mechanical drawings
- `/docs/` – Detailed project report, photos and videos of the assembled prototype

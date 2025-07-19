# IoT-Based-Conveyor-Weight-Sorting-System
## Problem Statement

Manual quality control in manufacturing—especially weight-based sorting—is often time-consuming, inconsistent, and prone to human error. This project aims to automate product weight validation and sorting using IoT-based sensors and actuators to improve efficiency and accuracy.

## Sensors Used

- Load Cell- 5 kg (via HX711 amplifier)
- Ultrasonic Sonar Sensor HC-SR04

## Actuators Used

- Micro Servo SG90 – 2 pcs
## Project Description

This automated system streamlines weight-based quality control on a rotating conveyor belt:

1. Items move along the belt and pass over a weight sensor that records weight.
2. Data is sent via ESP01S Wi-Fi module to a server.
3. Server checks if the weight is within limits.
4. Outside limits? The actuator rejects it off the belt.
5. IR sensor at the end ensures detection of misdirected items.
6. Second actuator sorts items into trays based on server command.

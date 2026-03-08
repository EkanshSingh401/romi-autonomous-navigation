# Romi Autonomous Navigation

Autonomous navigation system for the Pololu Romi robot using PID control and encoder feedback.

This project was developed for robotics experimentation and competition environments.

## Problem

Autonomous robots must navigate a predefined course without manual control. Reliable motion control requires precise motor regulation and feedback from encoders.

## Solution

This project implements a closed-loop control system using PID feedback to regulate motor speed and robot trajectory.

Encoder feedback is used to estimate wheel movement and adjust motor commands in real time.

## Architecture

Sensors / Encoders  
↓  
PID Control Loop  
↓  
Motor Commands  
↓  
Robot Motion

## Key Features

- PID motor control
- Encoder-based motion feedback
- Embedded control loop implementation
- Autonomous robot navigation logic

## Tech Stack

Arduino  
C++  
Embedded Systems  
Robotics Control  

## Hardware Platform

Pololu Romi 32U4 robot platform.

## Repository Structure

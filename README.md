# Romi Autonomous Navigation

Autonomous navigation system for the Pololu Romi robot using PID control and encoder feedback.

This project implements an embedded control system that enables a Romi robot to move autonomously using motor control, encoder feedback, and PID regulation.

## Problem

Autonomous robots must navigate a predefined course without manual control. Reliable motion requires accurate motor control and feedback from sensors to maintain trajectory and correct errors.

## Solution

This project implements a closed-loop control system using PID controllers to regulate motor behavior. Encoder feedback is used to track wheel motion and continuously adjust motor commands.

The system enables the robot to perform controlled motion using embedded control logic.

## Architecture

Encoder Feedback  
↓  
PID Control Loop  
↓  
Motor Commands  
↓  
Robot Motion  

## Key Features

- PID-based motor control
- Encoder feedback for motion tracking
- Embedded control loop implementation
- Autonomous robot navigation logic

## Hardware Platform

Pololu Romi 32U4 robot platform.

## Tech Stack

Arduino  
C++  
Embedded Systems  
Robotics Control  

## Repository Structure

romi-autonomous-navigation/
├ README.md
├ LICENSE
├ .gitignore
└ Romi_Code/
   ├ Romi_Code.ino
   ├ Chassis.cpp
   ├ Chassis.h
   ├ PIDcontroller.cpp
   ├ PIDcontroller.h
   ├ Pushbutton.cpp
   ├ Pushbutton.h
   ├ Timer.cpp
   ├ Timer.h
   ├ Romi32U4Motors.cpp
   ├ Romi32U4Motors.h
   ├ Romi32U4Encoders.cpp
   ├ Romi32U4Buttons.h
   ├ FastGPIO.h
   ├ USBPause.h
   ├ pcint.cpp
   └ pcint.h

## Notes

This repository contains firmware and control components used for embedded robot navigation experiments on the Pololu Romi platform.

# ECE4180Project

# Color Following Robot
ECE 4180 Final Project Spring 2024 <br />
Project Members: Matthew Chen, Shibao Su, Essie(Xinyi) Cao, Yash Fichadia

# Summary
Our goal is to put together a robot with autonomous color following capabilities, where a user can select a specific color, and the robot will autonomously move towards detections of that specific color, stopping when it reaches a certain distance threshold from that object. It will use a Raspberry Pi 4 with a PiCamera running an OpenCV powered project in real time, communicating with an mbed with an h-bridge to drive DC motors. There will also be an ultrasonic sensor indicating distances from nearby objects, using various speaker frequencies, for the safety of the robot. This robot will also have the option to be manually remote controlled, using a bluetooth module and a pushbutton to change modes between manual bluetooth control and autonomous color following.

# Parts List
- Raspberry Pi 4 (https://www.raspberrypi.com/products/raspberry-pi-4-model-b/)
- mbed LPC1768 (https://os.mbed.com/platforms/mbed-LPC1768/)
- Pi Camera (https://www.raspberrypi.com/documentation/computers/camera_software.html)
- Ultrasonic sensor (https://os.mbed.com/users/4180_1/notebook/using-the-hc-sr04-sonar-sensor/)
- Speaker (https://os.mbed.com/users/4180_1/notebook/using-a-speaker-for-audio-output/)
- H Bridge Motor Driver (https://os.mbed.com/users/4180_1/notebook/relays1/)
- Class D Amplifier (https://os.mbed.com/users/4180_1/notebook/tpa2005d1-class-d-audio-amp/)
- Bluetooth Module (https://os.mbed.com/users/4180_1/notebook/adafruit-bluefruit-le-uart-friend---bluetooth-low-/)
- DC motors (x2)
- Breadboard Power Supply Module
- Breadboard Barrel Jack Adapter

# Setup
Here is the block diagram:
![Block Diagram](images/block_diagram.png)

Include wiring details
Include power/ battery details

# Instructions

The backbone of this project is serial communication from the Raspberry Pi to the mbed, transferring movement commands in a very similar format to bluetooth command to autonomously control the car. There are two scripts that need to be running for this project to work, the first being object_detection.py, which is running on the Raspberry Pi, filtering out all pixels that aren't of a certain color and determining the center of mass of the remaining detected object. The second script is ECEFinalProject.cpp, which reads in instructions using multithreading, either from the serial port or bluetooth, and drives the robot forwards, backwards, left, right, or stops the robot based on the instructions. It also reads the values from an ultrasonic sensor to tell the distance from upcoming objects and plays a speaker at a certain frequency depending on distance from said objects.

For the autonomous mode, multiple parts work together, including the Raspberry Pi, the Pi Camera, the Ultrasonic sensor, the speaker, the H Bridge motor driver, DC motors, the Class D amplifier, and some barrel jacks/ external power supplies. The Raspberry Pi runs a script with OpenCV to determine the next instruction for the robot to take, the Pi Camera provides data for this script, the mbed reads data from the Raspberry Pi, and uses the H Bridge to drive the DC motors either forward or backward to achive 4 movement cases, forwards, backwards, left, or right. The motors are powered off of a 9V battery as a power supply. The ultrasonic sensor and the speaker also form a section of this mode, continuously beeping at different frequencies to indicate an object's distance to any object in the ultrasonic sensor's view. In turn, the manual mode uses many of the same parts, though it differs significantly in the control scheme. Instead, in this mode tohe car relies on fast responses/ commands from the connected bluefruit adafruit app. This app sends commands to a switch case statement which will detmerine if the car should move forwards, backwards, left, or right, depeding on the variable values for each motor. These motors are defined and set separately in the code so that they can move independently, else the car would only be able to move forwards and backwards.

# Media
- Presentation Slides (https://docs.google.com/presentation/d/12a98qyBk1TE1SFsfPouObr0URmi0DXX__ShfMNphmeI/edit?usp=sharing)

- Video of the two modes: 
https://youtu.be/m48Wb4iO9ZM
https://youtu.be/f1ZGfoOcz-w


# Conclusion
The autonomous color following robot is currently in a very early development stages, with the fundamental ability to follow a user selected color. While still preliminary, this robot holds immense potential for enhancement and adaptation across various sectors, including warehousing, healthcare, home automation and education. In addition to enhancing operational efficiencies, this robot will also introduce new possibilities for automation in industrial environments or everyday life at home by continuing to refine its capabilities such as grabbing items and lifting heavy weights.

#!/usr/bin/python3

import serial
import cv2
import numpy as np

from picamera2 import Picamera2

# Grab images as numpy arrays and leave everything else to OpenCV.

# face_detector = cv2.CascadeClassifier("/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml")
cv2.startWindowThread()

width = 640
height = 480
robot_x = width / 2
robot_y = height / 2

# Start picamera2 and change aspect ratio to 640 by 480
picam2 = Picamera2()
picam2.configure(picam2.create_preview_configuration(main={"format": 'XRGB8888', "size": (width, height)}))
picam2.start()

# Define the target hue value for object detection (in HSV color space)
HUE_VAL = 37

# Define the lower and upper bounds for the target color
lower_color = np.array([HUE_VAL - 20, 3, 3])
upper_color = np.array([HUE_VAL + 20, 255, 255])

# Initialize serial for mbed connection
ser = serial.Serial('/dev/ttyACM0', 9600)

while True:
    # Get image from picamera2
    image = picam2.capture_array()
    
    # Denoise image (not implemented)
    

    # Convert to HSV color space
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    
    # Create mask to filter colors in range of the target value
    color_mask = cv2.inRange(hsv, lower_color, upper_color)
    
    # Apply mask onto current image
    res = cv2.bitwise_and(image, image, mask=color_mask)
    
    # Find contour regions within the masked image
    contours, hierarchy = cv2.findContours(color_mask, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    
    # Initialize variables to track the largest object
    object_area = 0
    object_x = 0
    object_y = 0

    # Loop through detected contours
    for contour in contours:
        x, y, width, height = cv2.boundingRect(contour)
        found_area = width * height
        center_x = x + (width / 2)
        center_y = y + (height / 2)
        
        # Check if the current object is larger than the previous largest
        if object_area < found_area:
            object_area = found_area
            object_x = center_x
            object_y = center_y

    # Check if an object was found larger than margin of error for noise
    if object_area > 200:
        # Mark target object location on image with dot
        object_location = [object_area, object_x, object_y]
        res = cv2.circle(res, (int(object_x), int(object_y)), 3, (255, 255, 255), -1)
    else:
        object_location = None
        
    # Send movement command to mbed
    if object_area > 200:
        if object_x - robot_x > 100:
            ser.write('!B71'.encode('utf-8'))
            print('Turn left: ' + str(object_x - robot_x) + '\n')
        elif object_x - robot_x < -100:
            ser.write('!B81'.encode('utf-8'))
            print('Turn right: ' + str(object_x - robot_x) + '\n')
        else:
            if object_area < 220000:
                ser.write('!B51'.encode('utf-8'))
                print('Forward: ' + str(object_area) + '\n')
            elif object_area > 225000:
                ser.write('!B61'.encode('utf-8'))
                print('Backward: ' + str(object_area) + '\n')
            else:
                ser.write('!B11'.encode('utf-8'))
                print('Stop: ' + str(object_area) + '\n')
    else:
        ser.write('!B11'.encode('utf-8'))
        print('Stop: ' + str(object_area) + '\n')

    # Display image with or without detected object
    # cv2.imshow("Camera", res)
    cv2.waitKey(1)

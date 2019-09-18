# Assistance_for_visually_impaired

In the folder named "Project_code", there is the code for the arduino on the blind stick. Then it has a "colordetection" folder which can detect the color red. This can help the visually impaired to know if the signal in the crossing is red and he has to stop before crossing. The "facialreco_project" has a facial recognition model to detect known faces to help the visual impaired to know if someone he/she knows is around.
The "car_tracking" folder here has the code for tracking a moving vehicle. This helps the person to cross the road.

The blind stick is mounted by a couple sensors:
1) Gyro plus Pressure sensor: The gyro plus pressure sensor helps in fall detection. The pressure sensor helps in determining if the person is holding the stick when the gyro sensor already has detected the fall. This extra feature helps when the stick has fallen down but the person isn't holding it.

2) Ultrasonic sensor: This helps in knowing the closeness of an object to the stick and in return giving the person a sense of the distances between him and the objects in his surroundings.

3) Gesture sensor: This helps in any emergency actions. For example in the code given, we mapped a LEFT-RIGHT gesture detected, to a phone call to an emergecy contact.

4) LDR sensor: This helps to give information the light intensity of the area he/she is present in.

The raspberry pi which is connected to the arduino via bluetooth communicate with each other. 

The pi also has mic and it has a speech recognition model which can be mapped and used with own customizations.

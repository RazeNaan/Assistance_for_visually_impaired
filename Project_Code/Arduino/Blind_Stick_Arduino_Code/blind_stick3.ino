/**********************
GestureTest.ino
APDS-9960 RGB and Gesture Sensor
Shawn Hymel @ SparkFun Electronics
May 30, 2014
https://github.com/sparkfun/APDS-9960_RGB_and_Gesture_Sensor

Tests the gesture sensing abilities of the APDS-9960. Configures
APDS-9960 over I2C and waits for gesture events. Calculates the
direction of the swipe (up, down, left, right) and displays it
on a serial console. 

To perform a NEAR gesture, hold your hand
far above the sensor and move it close to the sensor (within 2
inches). Hold your hand there for at least 1 second and move it
away.

To perform a FAR gesture, hold your hand within 2 inches of the
sensor for at least 1 second and then move it above (out of
range) of the sensor.

Hardware Connections:

IMPORTANT: The APDS-9960 can only accept 3.3V!

 Arduino Pin  APDS-9960 Board  Function

 3.3V         VCC              Power
 GND          GND              Ground
 A4           SDA              I2C Data
 A5           SCL              I2C Clock
 2            INT              Interrupt

Resources:
Include Wire.h and SparkFun_APDS-9960.h

Development environment specifics:
Written in Arduino 1.0.5
Tested with SparkFun Arduino Pro Mini 3.3V

This code is beerware; if you see me (or any other SparkFun 
employee) at the local, and you've found our code helpful, please
buy us a round!

Distributed as-is; no warranty is given.
**********************/

#include <Wire.h>
#include <SparkFun_APDS9960.h>

// gyro

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
float ax=0, ay=0, az=0, gx=0, gy=0, gz=0;

//int data[STORE_SIZE][5]; //array for saving past data
//byte currentIndex=0; //stores current data array index (0-255)
boolean fall = false; //stores if a fall has occurred
boolean trigger1=false; //stores if first trigger (lower threshold) has occurred
boolean trigger2=false; //stores if second trigger (upper threshold) has occurred
boolean trigger3=false; //stores if third trigger (orientation change) has occurred

byte trigger1count=0; //stores the counts past since trigger 1 was set true
byte trigger2count=0; //stores the counts past since trigger 2 was set true
byte trigger3count=0; //stores the counts past since trigger 3 was set true
int angleChange=0;


// Pins
#define APDS9960_INT    2 // Needs to be an interrupt pin

// Constants

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;
const int trigger = 3; //Trigger pin of 1st Sesnor
const int echo = 4; //Echo pin of 1st Sesnor
const int Buzz = 13; //Echo pin of 1st Sesnor
const int Remote = A0; //Echo pin of 1st Sesnor
const int Light = A1; //Echo pin of 1st Sesnor
const int Pressure = A2; // Pin for pressure sensor
const int swtch = 9;
long time_taken;
int dist;
int Signal;
int ps; // pressure sensor
int Intens;
int similar_count;
int l = 0, r = 0;
void setup() {


  // gyro

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);

  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);

  
  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));

  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
//  Serial.begin(9600); 
pinMode(Buzz,OUTPUT);
digitalWrite(Buzz,LOW);
pinMode(trigger, OUTPUT); 
pinMode(echo, INPUT); 
}
void calculate_distance(int trigger, int echo)
{
digitalWrite(trigger, LOW);
delayMicroseconds(2);
digitalWrite(trigger, HIGH);
delayMicroseconds(10);
digitalWrite(trigger, LOW);

time_taken = pulseIn(echo, HIGH);
dist= time_taken*0.034/2;
if (dist>300)
dist=300;
}
void loop() {
 
  if(digitalRead(swtch) == 1)
  {
       
  if( isr_flag == 1 ) {
    detachInterrupt(0);
    handleGesture();
    attachInterrupt(0, interruptRoutine, FALLING);
  }
  }
  else
  { 
     ps = analogRead( Pressure);
    stick();
    gyro();
  }
}

void interruptRoutine() {
  isr_flag = 1;
}

void handleGesture() {
    if ( apds.isGestureAvailable() ) {

    switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial.println("UP");
          l = 0;
          r = 0;
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
          l = 0;
          r = 0;
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
          l = 1;
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
          r = 1;
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
          l = 0;
          r = 0;
        break;
      case DIR_FAR:
        Serial.println("FAR");
          l = 0;
          r = 0;
        break;
      default:
        Serial.println("NONE");
          l = 0;
          r = 0;
    }
    if(l == 1 || r == 1)
    {
      Serial.println("gesture detected");
    }
  }
}
void stick() { //infinite loopy
calculate_distance(trigger,echo);
Signal = analogRead(Remote);
Intens = analogRead(Light);


//If very dark
if (Intens<20)
{

  digitalWrite(Buzz,HIGH);delay(200);digitalWrite(Buzz,LOW);delay(200);digitalWrite(Buzz,HIGH);delay(200);

  digitalWrite(Buzz,LOW);delay(200);
  delay(500);
}


if (dist<50)
{

  
  digitalWrite(Buzz,HIGH);
  for (int i=dist; i>0; i--)
    delay(10);

  digitalWrite(Buzz,LOW);
  for (int i=dist; i>0; i--)
    delay(10);
    
}


}

// function of gyro

void mpu_read(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  }

  void gyro()
  {
      mpu_read();
  //2050, 77, 1947 are values for calibration of accelerometer
  // values may be different for you
  ax = (AcX-2050)/16384.00;
  ay = (AcY-77)/16384.00;
  az = (AcZ-1947)/16384.00;
  
  //270, 351, 136 for gyroscope
  gx = (GyX+270)/131.07;
  gy = (GyY-351)/131.07;
  gz = (GyZ+136)/131.07;
  
  // calculating Amplitute vactor for 3 axis
  float Raw_AM = pow(pow(ax,2)+pow(ay,2)+pow(az,2),0.5);
  int AM = Raw_AM * 10;  // as values are within 0 to 1, I multiplied 
                         // it by for using if else conditions 


  if (trigger3==true){
     trigger3count++;

     if (trigger3count>=6){ 
        angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5);
        Serial.println(angleChange); 
        if ((angleChange>=0) && (angleChange<=50)){ //if orientation changes remains between 0-10 degrees
            fall=true; trigger3=false; trigger3count=0;
            Serial.println(angleChange);
              }
        else{ //user regained normal orientation
           trigger3=false; trigger3count=0;
           Serial.println("TRIGGER 3 DEACTIVATED");
        }
      }
   }
  if (fall==true){ //in event of a fall detection
    if( ps > 25)
    {
       Serial.println("FALL DETECTED");
    }
    digitalWrite(11, LOW);
    delay(20);
    digitalWrite(11, HIGH);
    fall=false;
    }
  if (trigger2count>= 4){ //allow 0.5s for orientation change
    trigger2=false; trigger2count=0;

    }
  if (trigger1count>= 4){ //allow 0.5s for AM to break upper threshold
    trigger1=false; trigger1count=0;

    }
  if (trigger2==true){
    trigger2count++;
    angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5); Serial.println(angleChange);
    if (angleChange>=15 && angleChange<=600){ //if orientation changes by between 80-100 degrees
      trigger3=true; trigger2=false; trigger2count=0;

        }
    }
  if (trigger1==true){
    trigger1count++;
    if (AM>=6){ //if AM breaks upper threshold (3g)
      trigger2=true;
      trigger1=false; trigger1count=0;
      }
    }
  if (AM<=6 && trigger2==false){ //if AM breaks lower threshold (0.4g)
    trigger1=true;
    }
//It appears that delay is needed in order not to clog the port
  delay(100);
  }

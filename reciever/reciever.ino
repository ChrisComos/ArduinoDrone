#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>
#include <Servo.h>

Adafruit_MPU6050 mpu;
//create an RF24 object
RF24 radio(9, 10);  // CE, CSN
Servo myservo;
//address through which two modules communicate.
const byte address[6] = "00001";
typedef struct{
  float pitch;
  float roll;
  float yaw;
}
A_t;
A_t dataR;

 int milliOld, milliNew,dt;

 //k values for control function
 /*float k1 = .5;
 float k2 = 100;
 float k3 = .001;

 float rollTarget;
 float rollError = 0;
 float rollErrorOld;
 float rollErrorDif;       // error- errorold
 float rollErrorSlope = 0; //cahnge in error/change in time
 float rollErrorArea = 0;
 float rollVal;            //value of roll

 float pitchTarget;
 float pitchError = 0;
 float pitchErrorOld;
 float pitchErrorDif;       // error- errorold
 float pitchErrorSlope = 0; //cahnge in error/change in time
 float pitchErrorArea = 0;
 float pitchVal;            //value of pitch

 float yawTarget;
 float yawError = 0;
 float yawErrorOld;
 float yawErrorDif;         // error- errorold
 float yawErrorSlope = 0;  //cahnge in error/change in time
 float yawErrorArea = 0;
 float yawVal;           //value of yaw
 */

void setup()
{
  while (!Serial);
    Serial.begin(115200);
    Serial.print("Begin");
    // Try to initialize!
 if (!mpu.begin()) {
/*
Serial.println("Failed to find MPU6050 chip");
  while (1) {
     delay(10);
   }
*/
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);
  radio.begin();

  //set the address
  radio.openReadingPipe(0, address);

  //Set module as receiver
  radio.startListening();
}

void loop()
{
  //radio.startListening();
  //Read the data if available in buffer
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);


  double Dpitch = 180 * atan (a.acceleration.x/sqrt(a.acceleration.y*a.acceleration.y + a.acceleration.z*a.acceleration.z))/M_PI;
  double Droll = 180 * atan (a.acceleration.y/sqrt(a.acceleration.x*a.acceleration.x + a.acceleration.z*a.acceleration.z))/M_PI;
  double Dyaw = 180 * atan (a.acceleration.z/sqrt(a.acceleration.x*a.acceleration.x + a.acceleration.y*a.acceleration.y))/M_PI;


  if (radio.available())
  {
//  delay(1000);
    radio.read(&dataR, sizeof(dataR));
    printVals(dataR);
    if(dataR.pitch >= 0 && dataR.pitch < Dpitch){
      //left motors down
      }
    if(dataR.pitch >= 0 && dataR.pitch > Dpitch){
      //left motors ip
      }
    if(dataR.roll >= 0 && dataR.roll > Droll){
      //front motors up
      }
    if(dataR.roll >= 0 && dataR.roll < Droll){
      //front motors down
      }
    if(dataR.yaw >= 0 && dataR.yaw > Dyaw){
      //
      }
    if(dataR.yaw >= 0 && dataR.yaw < Dyaw){
      //
      }

    if(dataR.pitch >= 1 && dataR.pitch < Dpitch){
      //left motors up
      }
    if(dataR.pitch >= 1 && dataR.pitch > Dpitch){
      //left motors down
      }
    if(dataR.roll >= 1 && dataR.roll > Droll){
      //front motors down
      }
    if(dataR.roll >= 1 && dataR.roll < Droll){
      //front motors up
      }
    if(dataR.yaw >= 1 && dataR.yaw > Dyaw){
      //
      }
    if(dataR.yaw >= 1 && dataR.yaw < Dyaw){
      //
      }



/*
//    rollTarget = dataR.roll;      // attempt to get the
//    pitchTarget = dataR.pitch;
//    yawTarget = dataR.pitch;
//
//
//
//
//
//    milliOld = milliNew;        //set new old to old new
//    milliNew = millis();
//    dt = milliNew - milliOld;
//
//    rollErrorOld = rollError;
//    rollError = rollTarget-rollActual;
//    rollErrorDif = rollError - rollErrorOld;
//    rollErrorSlope = rollErrorDif/dt;
//    rollErrorArea = rollErrorArea + rollError*dt;
//
//    pitchErrorOld = pitchError;
//    pitchError = pitchTarget - pitchActual;
//    pitchErrorDif = pitchError - pitchErrorOld;
//    pitchErrorSlope = pitchErrorDif/dt;
//    pitchErrorArea = pitchErrorArea + pitchError*dt;
//
//    yawErrorOld = yawError;
//    yawError = yawTarget-yawActual;
//    yawErrorDif = yawError - yawErrorOld;
//    yawErrorSlope = yawErrorDif/dt;
//    yawErrorArea = yawErrorArea + yawError*dt;
//
//
//    rollVal = rollVal + k1*rollError + k2*rollErrorSlope + k3*rollErrorArea;
//    pitchVal = pitchVal + k1*pitchError + k2*pitchErrorSlope + k3*pitchErrorArea;
//    yawVal = yawVal + k1*yawError + k2*yawErrorSlope + k3*yawErrorArea;
//
//    printf("Roll: %f\t Pitch: %f\t Yaw: %f\t\n", rollVal, pitchVal, yawVal);
//
*/
  }
}





void printVals( A_t dataR){

  Serial.print("Pitch: ");
  Serial.print(dataR.pitch);
  Serial.print(", Roll: ");
  Serial.print(dataR.roll);
  Serial.print(", Yaw: ");
  Serial.print(dataR.yaw);
  Serial.print(" rad/s");
  Serial.println();
   }

void printOrientation(A_t dataR){

  String FB;
  String LR;
  String Far = "";
  if (dataR.pitch < 0){
    LR = "Right";
  }
  else{
    LR = "Left";
  }
  if (dataR.roll < 0){
    FB = "Forward";
  }
  else{
    FB = "Backward";
  }
  if(dataR.pitch < -45 || dataR.pitch > 45 || dataR.roll < -45 || dataR.roll > 45)
 {
  Far = "far";
 }

  Serial.print(FB);
  Serial.print(" ");
  Serial.print(LR);
  Serial.print(" ");
  Serial.println(Far);
}

//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

Adafruit_MPU6050 mpu;

//create an RF24 object
RF24 radio(9, 10);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";
typedef struct{
  float pitch;
  float roll;
  float yaw;
}
A_t;
A_t dataS;
void setup()
{
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
 if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
   while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);


  
  radio.begin();
  //set the address
  radio.openWritingPipe(address);
  
  //Set module as transmitter
  radio.stopListening();
}




void loop()
{
  radio.stopListening();
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
//  const int array1[3] = {g.gyro.x*100, g.gyro.y*100, g.gyro.z*100};


  dataS.pitch = 180 * atan (a.acceleration.x/sqrt(a.acceleration.y*a.acceleration.y + a.acceleration.z*a.acceleration.z))/M_PI;
  dataS.roll = 180 * atan (a.acceleration.y/sqrt(a.acceleration.x*a.acceleration.x + a.acceleration.z*a.acceleration.z))/M_PI;
  dataS.yaw = 180 * atan (a.acceleration.z/sqrt(a.acceleration.x*a.acceleration.x + a.acceleration.y*a.acceleration.y))/M_PI;


  
  //Send message to receiver
  //const int button = digitalRead(4);
//  Serial.print(  );
  double place;
//  delay(1000);
  radio.write(&dataS, sizeof(dataS));
  
    Serial.print("Pitch: ");
  Serial.print(dataS.pitch);
  Serial.print(", Roll: ");
  Serial.print(dataS.roll);
  Serial.print(", Yaw: ");
  Serial.print(dataS.yaw);
  Serial.println(" rad/s");
 
}

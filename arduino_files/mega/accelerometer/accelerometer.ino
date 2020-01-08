#include <MPU9250_asukiaaa.h>
#include "math.h"
MPU9250_asukiaaa mySensor;
#define sampleTime  0.002

float gX, gY, gZ,gyroAngle,accAngle,gyroRate;
float aX, aY, aZ, aSqrt;
unsigned long currTime, prevTime=0, loopTime;
float currentAngle, prevAngle=0;
long sampleTime_us = 2000;
long lastTime;
int loopNum=0;
uint8_t inp_vec[7] = {0,0,0,0,0,0,0}; 

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  
  Wire.begin();
  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();

  lastTime = micros();

}

void send_packet(float arg){
  byte* current_angle_byte_array = (byte *)& arg;
  byte prefix = 0x00;
  byte postfix = 0xFF;
  Serial.write(prefix);
//  uint16_t aa = (uint16_t)(arg *100);
//  Serial.write((uint8_t)((aa>>8)&0xFF));
//  Serial.write((uint8_t)(aa&0xFF));
  Serial.write(current_angle_byte_array, sizeof(arg));
  Serial.write(postfix);
}

void loop() {
  mySensor.accelUpdate();
  aX = mySensor.accelX();
  aY = mySensor.accelY();
  aZ = mySensor.accelZ();
  aSqrt = mySensor.accelSqrt();
  
  //gyro
//    currTime = millis();
//  loopTime = currTime - prevTime;
//  prevTime = currTime;
  
  mySensor.gyroUpdate();
  gX = mySensor.gyroX();
  gY = mySensor.gyroY();
  gZ = mySensor.gyroZ();

  accAngle = atan2(aY, aZ)*RAD_TO_DEG;

// accAngle = atan2(aZ, aY)*RAD_TO_DEG;
//
//accAngle += 90;



//accAngle = -accAngle;/
//  
//  if (accAngle > 0)
//    accAngle -= 180;
//  else
//    accAngle += 180;

//  accAngl/e = -accAngle;
//  accAngle = (abs(accAngle) - 180.0)*/
  
  gyroRate = map(gX, -32768, 32767, -250, 250);
  gyroAngle = (float)gyroRate*sampleTime;  
//  currentAngle = 0.9934*(currentAngle + gyroAngle) + 0.0066*(accAngle);

  currentAngle = 0.9*(currentAngle + gyroAngle) + 0.1*(accAngle);
  
  prevAngle = currentAngle;
  
  loopNum++;
  if(loopNum > 4)
  {
    loopNum = 0;
    send_packet(currentAngle);
  }
  
  while( abs(micros() - lastTime) < sampleTime_us){}
  lastTime = micros();
  
}



void serialEvent()
{
  while (Serial.available()) {
    // get the new byte:
    uint8_t inChar = (uint8_t)Serial.read();
    inp_vec[0] = inp_vec[1]; 
    inp_vec[1] = inp_vec[2];
    inp_vec[2] = inp_vec[3]; 
    inp_vec[3] = inp_vec[4];
    inp_vec[4] = inp_vec[5]; 
    inp_vec[5] = inp_vec[6];
    inp_vec[6] = inChar;
    if (inp_vec[0]==0 && inp_vec[1]==0 && inp_vec[6]==255)
    {
  //    int16_t l_motor_speed = ((int16_t)(inp_vec[3]<<8 | inp_vec[2]));
//      Serial.println();
//      Serial.println(l_motor_speed);
      Serial2.write(inp_vec, 7);
      Serial2.flush();
      Serial.flush();
    }
  
  }
}

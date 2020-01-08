
#include <DynamixelWorkbench.h>

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif   

#define BAUDRATE  1000000
#define DXL_ID    3

int32_t motor_speed=200;
int32_t l_motor_speed=0 , r_motor_speed=0;


int u;

  uint8_t dxl_id_left = 3;
  uint8_t dxl_id_right = 4 , dxl_id = 4;
  uint16_t model_number = 0;

DynamixelWorkbench dxl_wb;


uint8_t inp_vec[7] = {0,0,0,0,0,0,0}; 


void setup() 
{  
  Serial.begin(57600);
  // while(!Serial); // Wait for Opening Serial Monitor
Serial2.begin(115200);
  const char *log;
  bool result = false;



  result = dxl_wb.init(DEVICE_NAME, BAUDRATE, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to init");
  }
  else
  {
    Serial.print("Succeeded to init : ");
    Serial.println(BAUDRATE);  
  }

  result = dxl_wb.ping(dxl_id_left, &model_number, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to ping dxl_id_left");
  }
  else
  {
    Serial.println("Succeeded to ping dxl_id_left");
    Serial.print("id : ");
    Serial.print(dxl_id_left);
    Serial.print(" model_number : ");
    Serial.println(model_number);
  }

  result = dxl_wb.ping(dxl_id_right, &model_number, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to ping dxl_id_right");
  }
  else
  {
    Serial.println("Succeeded to ping dxl_id_right");
    Serial.print("id : ");
    Serial.print(dxl_id_right);
    Serial.print(" model_number : ");
    Serial.println(model_number);
  }
  
  result = dxl_wb.wheelMode(dxl_id_right, 0, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to change wheel mode dxl_id_right");
  }
  else
  {
    Serial.println("Succeed to change wheel mode dxl_id_right");
    Serial.println("Dynamixel is moving...");
  }
  result = dxl_wb.wheelMode(dxl_id_left, 0, &log);
  if (result == false)
  {
    Serial.println(log);
    Serial.println("Failed to change wheel mode dxl_id_left");
  }
  else
  {
    Serial.println("Succeed to change wheel mode dxl_id_left");
    Serial.println("Dynamixel is moving...");
  }
}

void loop() 
{
    dxl_wb.goalVelocity((uint8_t)dxl_id_left, (int32_t)l_motor_speed);
    dxl_wb.goalVelocity((uint8_t)dxl_id_right, (int32_t)r_motor_speed);
    delay(5);
}


void serialEvent2() {
  /*
  while (Serial2.available()) {
    // get the new byte:
    char inChar = (char)Serial2.read();
    u = (int)inChar-127;
   motor_speed = (int32_t)map(u,-128,128,-1000,1000);
    Serial.println(motor_speed);
  }
    */



  while (Serial2.available()) {
    // get the new byte:
    uint8_t inChar = (uint8_t)Serial2.read();
    inp_vec[0] = inp_vec[1]; 
    inp_vec[1] = inp_vec[2];
    inp_vec[2] = inp_vec[3]; 
    inp_vec[3] = inp_vec[4];
    inp_vec[4] = inp_vec[5]; 
    inp_vec[5] = inp_vec[6];
    inp_vec[6] = inChar;
    if (inp_vec[0]==0 && inp_vec[1]==0 && inp_vec[6]==255)
    {
      l_motor_speed = (int32_t)((int16_t)(inp_vec[3]<<8 | inp_vec[2]));
      r_motor_speed = -(int32_t)((int16_t)(inp_vec[5]<<8 | inp_vec[4]));

      l_motor_speed = constrain(l_motor_speed , -1023 , 1023);
      r_motor_speed = constrain(r_motor_speed , -1023 , 1023);

      Serial2.flush();
    }
  
  }
  

  
}

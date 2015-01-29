/* Author : AkhilHector (Hehahahahahha) , Om_Ethcelon
Circuit : Suraj
--Gesture controlled mine field bot
Ce pin 7
Csn pin 6
Mosi pin
Miso pin
Sck pin
*/
#include <Wire.h>
#include <RF24.h>
#include <SPI.h>
#include <RF24Network.h>

/* the motor section */

#define x_pin A3
#define y_pin A2
#define z_pin A1
#define rad_to_pi 180/3.14159
/* The i2c address register for the MPU */
#define MPU6050 0X68
#define MPU6050_me 0X75
/* The Power MGT register */
#define MPU6050_pwr_mgt_1 0X6B
#define MPU6050_pwr_mgt_2 0X6C
typedef union accel_t_gyro_union
{
struct
{
uint8_t x_accel_h;
uint8_t x_accel_l;
uint8_t y_accel_h;
uint8_t y_accel_l;
uint8_t z_accel_h;
uint8_t z_accel_l;
uint8_t x_gyro_h;
uint8_t x_gyro_l;
uint8_t y_gyro_h;
uint8_t y_gyro_l;
uint8_t z_gyro_h;
uint8_t z_gyro_l;
} reg;
struct
{
int x_accel;
int y_accel;
int z_accel;
int x_gyro;
int y_gyro;
int z_gyro;
} value;
};
int x_accl;
int y_accl;
int z_accl;
int x_gyro;
int y_gyro;
int z_gyro;


float z_foffset_h = -2600.00;
float z_foffset_l = -17000.00;
float z_boffset_h = 15000.00;
float z_boffset_l = 3000.00;
float x_offset_h = 10.00;
float x_offset_l = -10.00;
float y_offset_h = -75.00;
float y_offset_l = -90.00;
float x_roffset_h = -30.00;
float x_roffset_l = -90.00;
float y_roffset_h = 5.00;
float y_roffset_l = -76.00;
float x_loffset_h = 85.00;
float x_loffset_l = 12.00;
float y_loffset_h = 5.00;
float y_loffset_l = -80.00;

float ax_boffset_h = 25.00;
float ax_boffset_l = 23.00;
float ay_boffset_h = -24.00;
float ay_boffset_l = -27.00;
float ax_roffset_h = 39.00;
float ax_roffset_l = 27.50;
float ay_roffset_h = -5.00;
float ay_roffset_l = -26.00;
float ay_loffset_h = -31.00;
float ay_loffset_l = -55.00;

float az_foffset_h =  23.91;
float az_foffset_l =  5.00;

float az_boffset_h = 55.00;
float az_boffset_l = 33.00;


unsigned long last_read_time;
float last_x_angle;
float last_y_angle;
float last_z_angle;
float last_gyro_x_angle;
float last_gyro_y_angle;
float last_gyro_z_angle;

void set_last_read_angle_data(unsigned long time, float x, float y, float z, float x_gyro, float y_gyro, float z_gyro) {
      last_read_time = time;
      last_x_angle = x;
      last_y_angle = y;
      last_z_angle = z;
      last_gyro_x_angle = x_gyro;
      last_gyro_y_angle = y_gyro;
      last_gyro_z_angle = z_gyro;
}


inline unsigned long get_last_time() {return last_read_time;}
inline float get_last_x_angle() {return last_x_angle;}
inline float get_last_y_angle() {return last_y_angle;}
inline float get_last_z_angle() {return last_z_angle;}
inline float get_last_gyro_x_angle() {return last_gyro_x_angle;}
inline float get_last_gyro_y_angle() {return last_gyro_y_angle;}
inline float get_last_gyro_z_angle() {return last_gyro_z_angle;}


float base_x_accel;
float base_y_accel;
float base_z_accel;
float base_x_gyro;
float base_y_gyro;
float base_z_gyro;


int read_gyro_accel_vals(uint8_t* accel_t_gyro_ptr) 
{
  accel_t_gyro_union* accel_t_gyro = (accel_t_gyro_union *) accel_t_gyro_ptr;
  int error = mpuread (MPU6050_ACCEL_XOUT_H, (uint8_t *) accel_t_gyro, sizeof(*accel_t_gyro));

uint8_t swap;
#define SWAP(x,y) swap = x; x = y; y = swap
SWAP ((*accel_t_gyro).reg.x_accel_h, (*accel_t_gyro).reg.x_accel_l);
SWAP ((*accel_t_gyro).reg.y_accel_h, (*accel_t_gyro).reg.y_accel_l);
SWAP ((*accel_t_gyro).reg.z_accel_h, (*accel_t_gyro).reg.z_accel_l);
SWAP ((*accel_t_gyro).reg.x_gyro_h, (*accel_t_gyro).reg.x_gyro_l);
SWAP ((*accel_t_gyro).reg.y_gyro_h, (*accel_t_gyro).reg.y_gyro_l);
SWAP ((*accel_t_gyro).reg.z_gyro_h, (*accel_t_gyro).reg.z_gyro_l);
return error;
}


void calibrate_sensors() 
{
      int num_readings = 10;
      float x_accel = 0;
      float y_accel = 0;
      float z_accel = 0;
      float x_gyro = 0;
      float y_gyro = 0;
      float z_gyro = 0;
    accel_t_gyro_union accel_t_gyro;
        Serial.println("Starting Calibration");
    read_gyro_accel_vals((uint8_t *) &accel_t_gyro);
      for (int i = 0; i < num_readings; i++)
      {
      read_gyro_accel_vals((uint8_t *) &accel_t_gyro);

      x_accel += accel_t_gyro.value.x_accel;
      y_accel += accel_t_gyro.value.y_accel;
      z_accel += accel_t_gyro.value.z_accel;
      x_gyro += accel_t_gyro.value.x_gyro;
      y_gyro += accel_t_gyro.value.y_gyro;
      z_gyro += accel_t_gyro.value.z_gyro;
  delay(100);
}
      x_accel /= num_readings;
      y_accel /= num_readings;
      z_accel /= num_readings;

      x_gyro /= num_readings;
      y_gyro /= num_readings;
      z_gyro /= num_readings;

      base_x_accel = x_accel;
      base_y_accel = y_accel;
      base_z_accel = z_accel;

      base_x_gyro = x_gyro;
      base_y_gyro = y_gyro;
      base_z_gyro = z_gyro;
  Serial.println("Finishing Calibration");
}

RF24 radio(7,6);
const uint64_t pipe = 0xE8E8F0F0E1LL;
uint8_t states[2];

      
void setup()
{
        states[1] = 0;
      int store;
      uint8_t c;
      Serial.begin(115200);
  Wire.begin();
  radio.begin();
      radio.printDetails();
      radio.openWritingPipe(pipe);
    store = mpuread (MPU6050_WHO_AM_I, &c, 1);
    store = mpuread (MPU6050_PWR_MGMT_2, &c, 1);
      writereg (MPU6050_PWR_MGMT_1, 0);
      calibrate_sensors();
      set_last_read_angle_data(millis(), 0, 0, 0, 0, 0, 0);
}
void loop()
{
    initiate();
      Serial.print("states::\t");
      Serial.print(states[0]);
      Serial.print("\t");
      Serial.println(states[1]);
  bool ok = radio.write(states, 2);
      if(ok)
        Serial.println("ok");
      else
        Serial.println("failed");
        delay(500);  // remove 
}
void initiate()
{
  
    int store;
    double dT;
    
    unsigned long t = millis();
  accel_t_gyro_union accel_t_gyro;
  store = read_gyro_accel_vals((uint8_t*) &accel_t_gyro);
    unsigned long t_now = millis();
      float gyro_x = (accel_t_gyro.value.x_gyro - base_x_gyro)/131;
      float gyro_y = (accel_t_gyro.value.y_gyro - base_y_gyro)/131;
      float gyro_z = (accel_t_gyro.value.z_gyro - base_z_gyro)/131;

      float accel_x = accel_t_gyro.value.x_accel;
      float accel_y = accel_t_gyro.value.y_accel;
      float accel_z = accel_t_gyro.value.z_accel;

      float accel_angle_y = atan(-1*accel_x/sqrt(pow(accel_y,2) + pow(accel_z,2)))*rad_to_pi;
      float accel_angle_x = atan(accel_y/sqrt(pow(accel_x,2) + pow(accel_z,2)))*rad_to_pi;
      float accel_angle_z = 0;

      float dt =(t_now - get_last_time())/1000.0;
      float gyro_angle_x = gyro_x*dt + get_last_x_angle();
      float gyro_angle_y = gyro_y*dt + get_last_y_angle();
      float gyro_angle_z = gyro_z*dt + get_last_z_angle();
      float unfiltered_gyro_angle_x = gyro_x*dt + get_last_gyro_x_angle();
      float unfiltered_gyro_angle_y = gyro_y*dt + get_last_gyro_y_angle();
      float unfiltered_gyro_angle_z = gyro_z*dt + get_last_gyro_z_angle();

      float alpha = 0.96;
      float angle_x = alpha*gyro_angle_x + (1.0 - alpha)*accel_angle_x;
      float angle_y = alpha*gyro_angle_y + (1.0 - alpha)*accel_angle_y;
      float angle_z = gyro_angle_z;
      
      uint8_t x_pin_val = analogRead(x_pin);
      uint8_t y_pin_val = analogRead(y_pin);
      uint8_t z_pin_val = analogRead(z_pin);
      
      float x_pin_ang = atan(y_pin_val/sqrt(pow(x_pin_val,2) + pow(z_pin_val,2)))*rad_to_pi;
      float y_pin_ang = atan(-1*x_pin_val/sqrt(pow(y_pin_val,2) + pow(z_pin_val,2)))*rad_to_pi;
      float z_pin_ang = 0;

      
      set_last_read_angle_data(t_now, angle_x, angle_y, angle_z, unfiltered_gyro_angle_x, unfiltered_gyro_angle_y, unfiltered_gyro_angle_z);
   
    if(accel_angle_x < x_offset_h && accel_angle_x > x_offset_l)
    {
      if(accel_angle_y < y_offset_h && accel_angle_y > y_offset_l)
      {
        states[0] = 5;
       // Serial.println("Init");  
        
  if(x_pin_ang > ax_boffset_l && x_pin_ang < ax_boffset_h)
  {
    if(y_pin_ang > ay_boffset_l && y_pin_ang < ay_boffset_h)
   {
     states[1] = 1;
    Serial.println("Hold");
   }
  }
 
   else if(y_pin_ang > ay_loffset_l && y_pin_ang < ay_loffset_h)
  {
    states[1] = 2;
   Serial.println("Grab");
  }
  else if(x_pin_ang > az_boffset_l && x_pin_ang < az_boffset_h)
  {
    states[1] = 3;
    Serial.println("Crawl Back");
  }
  else if(x_pin_ang > az_foffset_l && x_pin_ang < az_foffset_h)
  {
    states[1] = 4;
    Serial.print("Crawl Front");
  }
  else if(x_pin_ang > ax_roffset_l && x_pin_ang < ax_roffset_h)
  {
    if(y_pin_ang > ay_roffset_l && y_pin_ang < ay_roffset_h)
    {
      states[1] = 5;
      Serial.println("Loose up");
    }
  }
      // Serial.println(accel_z,2); //
       }
     }
    else if(accel_angle_x < x_roffset_h && accel_angle_x > x_roffset_l)
    {
      if(accel_angle_y < y_roffset_h && accel_angle_y > y_roffset_l)
      {
        states[0] = 2;
      //  Serial.println("Right");
      
  if(x_pin_ang > ax_boffset_l && x_pin_ang < ax_boffset_h)
  {
    if(y_pin_ang > ay_boffset_l && y_pin_ang < ay_boffset_h)
   {
    Serial.println("None");
    states[1] = 1;  
     }
  }
 
   else if(y_pin_ang > ay_loffset_l && y_pin_ang < ay_loffset_h)
  {
   Serial.println("Grab");
   states[1] = 2;
  }
  else if(x_pin_ang > az_boffset_l && x_pin_ang < az_boffset_h)
  {
    Serial.println("Crawl Back");
    states[1] = 3;
  }
  else if(x_pin_ang > az_foffset_l && x_pin_ang < az_foffset_h)
  {
    Serial.print("Crawl Front");
    states[1] = 4;
  }
  else if(x_pin_ang > ax_roffset_l && x_pin_ang < ax_roffset_h)
  {
    if(y_pin_ang > ay_roffset_l && y_pin_ang < ay_roffset_h)
    {
      states[1] = 5;
      Serial.println("Loose");
    }
  }
        
// Serial.println(accel_z,2); // angle y
      }
    }

    else if(accel_angle_x < x_loffset_h && accel_angle_x > x_loffset_l)
    {
      if(accel_angle_y < y_loffset_h && accel_angle_y > y_loffset_l)
      {
        states[0] = 3;
      //  Serial.println("Left");
      
  if(x_pin_ang > ax_boffset_l && x_pin_ang < ax_boffset_h)
  {
    if(y_pin_ang > ay_boffset_l && y_pin_ang < ay_boffset_h)
   {
     states[1] = 1;
    Serial.println("Hold");
   }
  }
 
   else if(y_pin_ang > ay_loffset_l && y_pin_ang < ay_loffset_h)
  {
    states[1] = 2;
   Serial.println("Grab");
  }
  else if(x_pin_ang > az_boffset_l && x_pin_ang < az_boffset_h)
  {
    states[1] = 3;
    Serial.println("Crawl Back");
  }
  else if(x_pin_ang > az_foffset_l && x_pin_ang < az_foffset_h)
  {
    states[1] = 4;
    Serial.print("Crawl Front");
  }
  else if(x_pin_ang > ax_roffset_l && x_pin_ang < ax_roffset_h)
  {
    if(y_pin_ang > ay_roffset_l && y_pin_ang < ay_roffset_h)
    {
      states[1] = 5;
      Serial.println("Loose up");
    }
  }
// Serial.println(accel_z,2); //
      }
    }
    
    else if(accel_z < z_foffset_h && accel_z > z_foffset_l)
    {
      states[0] = 1;
   //   Serial.println("Forward");
   
  if(x_pin_ang > ax_boffset_l && x_pin_ang < ax_boffset_h)
  {
    if(y_pin_ang > ay_boffset_l && y_pin_ang < ay_boffset_h)
   {
     states[1] = 1;
    Serial.println("Hold");
   }
  }
 
   else if(y_pin_ang > ay_loffset_l && y_pin_ang < ay_loffset_h)
  {
    states[1] = 2;
   Serial.println("Grab");
  }
  else if(x_pin_ang > az_boffset_l && x_pin_ang < az_boffset_h)
  {
    states[1] = 3;
    Serial.println("Crawl Back");
  }
  else if(x_pin_ang > az_foffset_l && x_pin_ang < az_foffset_h)
  {
    states[1] = 4;
    Serial.print("Crawl Front");
  }
  else if(x_pin_ang > ax_roffset_l && x_pin_ang < ax_roffset_h)
  {
    if(y_pin_ang > ay_roffset_l && y_pin_ang < ay_roffset_h)
    {
      states[1] = 5;
      Serial.println("Loose up");
    }
  }
    }

    else if(accel_z < z_boffset_h && accel_z > z_boffset_l)
    {

        states[0] = 4;
      //  Serial.println("Back");
      
  if(x_pin_ang > ax_boffset_l && x_pin_ang < ax_boffset_h)
  {
    if(y_pin_ang > ay_boffset_l && y_pin_ang < ay_boffset_h)
   {
     states[1] = 1;
    Serial.println("Hold");
   }
  }
 
   else if(y_pin_ang > ay_loffset_l && y_pin_ang < ay_loffset_h)
  {
    states[1] = 2;
   Serial.println("Grab");
  }
  else if(x_pin_ang > az_boffset_l && x_pin_ang < az_boffset_h)
  {
    states[1] = 3;
    Serial.println("Crawl Back");
  }
  else if(x_pin_ang > az_foffset_l && x_pin_ang < az_foffset_h)
  {
    states[1] = 4;
    Serial.print("Crawl Front");
  }
  else if(x_pin_ang > ax_roffset_l && x_pin_ang < ax_roffset_h)
  {
    if(y_pin_ang > ay_roffset_l && y_pin_ang < ay_roffset_h)
    {
      states[1] = 5;
      Serial.println("Loose up");
    }
  }
    }
/*
else
{
Serial.println("The Filtered angles obtained are ");
Serial.print(accel_angle_x, 2);
Serial.print(F(","));
Serial.print(accel_angle_y, 2);
Serial.println(F(","));
Serial.println(accel_z,2);
}
*/
}

void clasp()
{
  

}

void exec()
{
      Wire.write(MPU6050_pwr_mgt_1);
      Wire.endTransmission(false);
      Wire.requestFrom(MPU6050,12,true);

    x_accl = Wire.read()<< 8 | Wire.read();
    y_accl = Wire.read()<< 8 | Wire.read();
    z_accl = Wire.read()<< 8 | Wire.read();
    x_gyro = Wire.read()<< 8 | Wire.read();
    y_gyro = Wire.read()<< 8 | Wire.read();
    z_gyro = Wire.read()<< 8 | Wire.read();

Serial.println("The Gyro values are ");
Serial.println(x_accl);
Serial.println(y_accl);
Serial.println(z_accl);
Serial.println("The acclerometer values are");
Serial.println(x_gyro);
Serial.println(y_gyro);
Serial.println(z_gyro);

}

int writereg(int reg, uint8_t data)
{
int error;
error = mpuwrite(reg, &data, 1);
return (error);
}


int mpuread(int start, uint8_t *buffer, int size)
{
int i, n, error;
Wire.beginTransmission(MPU6050_I2C_ADDRESS);
n = Wire.write(start);
if (n != 1)
return (-10);
n = Wire.endTransmission(false);
if (n != 0)
return (n);
Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
i = 0;
while(Wire.available() && i<size)
{
buffer[i++]=Wire.read();
}
if ( i != size)
return (-11);
return (0);
}


int mpuwrite(int start, const uint8_t *pData, int size)
{
int n, error;
Wire.beginTransmission(MPU6050_I2C_ADDRESS);
n = Wire.write(start);
if (n != 1)
return (-20);
n = Wire.write(pData, size);
if (n != size)
return (-21);
error = Wire.endTransmission(true);
if (error != 0)
return (error);
return (0);
}

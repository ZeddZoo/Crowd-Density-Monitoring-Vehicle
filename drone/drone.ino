#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//define pins




//sensor
Adafruit_BNO055 bno = Adafruit_BNO055(55);
 
//translational velocities
float vx = 0;
float vy = 0;
float vz = 0;

//angle
float th = 0;

//starting time (secs)
unsigned long elapsedTime;

void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);

  //starting time (secs)
  elapsedTime = millis() / 1000;
}
 
void loop(void) 
{

  //Current time, update elapsed time
  unsigned long dt = (millis() / 1000) - elapsedTime;
  elapsedTime = elapsedTime + dt;

  //Get a new sensor event
  sensors_event_t event; 
  bno.getEvent(&event);

  //Gather data from sensors
  sensor_vec_t orient = event.orientation;
  imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu::Vector<3> linaccel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

  //refresh translational velocities using integration
  vx += (linaccel.x() * dt);
  vy += (linaccel.y() * dt);
  vz += (linaccel.z() * dt);


  
  delay(10); //100hz tasks
}

#include <Wire.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "controller.h"
#include "commands.h"

//define pins

//camera mount
#define CAM_PAN_PIN 7
#define CAM_TILT_PIN 8

//motors
#define M1_PIN
#define M2_PIN
#define M3_PIN
#define M4_PIN

//status toggle
#define STAT_TOGGLE_PIN


//sensor init
Adafruit_BNO055 bno = Adafruit_BNO055(55);

//servos init
Servo pan;
Servo tilt;

//servo angles
int pan_angle;
int tilt_angle;
 
//translational velocities
float vx = 0;
float vy = 0;
float vz = 0;

//angular velocity
float omega = 0;

//starting time (secs)
unsigned long elapsedTime;

//flags
bool landing_flag = true;
bool command_flag = false;

//calculated hover torque (0-255)
int hover_dutycycle = 64;

//setup
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

  //initialize servos
  pan.attach(CAM_PAN_PIN);
  tilt.attach(CAM_TILT_PIN);

  //get servo initial positions
  pan_angle = pan.read();
  tilt_angle = tilt.read();

  //starting time (secs)
  elapsedTime = millis() / 1000;
}
 
void loop(void) 
{
  //control if not on ground
  if (!stationary_flag){

    //Current time, update elapsed time
    unsigned long dt = (millis() / 1000) - elapsedTime;
    elapsedTime = elapsedTime + dt;

    //Get a new sensor event
    sensors_event_t event; 
    bno.getEvent(&event);

    //Gather data from sensors

    //absolute orientation is 0-360
    sensor_vec_t orient = event.orientation;

    //gyroscope and linear acceleration has negative values
    imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu::Vector<3> linaccel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

    //refresh translational and rotational velocities using integration
    vx += (linaccel.x() * dt);
    vy += (linaccel.y() * dt);
    vz += (linaccel.z() * dt);
    omega += (gyro.z() * dt);

    //take in commands
    int commands[4];

    /*
    * TODO
    */


    //determine if manual command is used, match command if issued, make drone stationary if not
    float orient_ref[3];
    float vz_ref;

    if (command_flag){
      //calculate reference orientation
      orient_ref[3] = xyz_to_orient_ref(commands);
      vz_ref = get_reference_vz(commands);
    }
    else{
      orient_ref[3] = cancel_translational_movement(vx, vy, vz);
      vz_ref = 0.0;
    }

    //get drone to match reference orientation
    int rot_corrections[3] = get_rotational_corrections(orient, gyro, orient_ref);
    int hover_corrections = get_hover_corrections(vz, vz_ref);

    //mix motors (TODO: tune motor mixing algo)
    int dutycycles[4] = mix_motors(hover_dutycycle, rot_corrections, hover_corrections);

    //write dutycycles to pins
    analogWrite(M1_PIN, dutycycles[0]);
    analogWrite(M2_PIN, dutycycles[1]);
    analogWrite(M3_PIN, dutycycles[2]);
    analogWrite(M4_PIN, dutycycles[3]);
  }
  else{
    //do not turn on motors
    analogWrite(M1_PIN, 0);
    analogWrite(M2_PIN, 0);
    analogWrite(M3_PIN, 0);
    analogWrite(M4_PIN, 0);
  }
  //move servos to their position
  
  //read commands: TODO Implement
  int camera_commands[2];
  //read commands


  //change pan angle
  if (camera_commands[0] < 0){
    pan_angle -= 5;
  }
  else if (camera_commands[0] > 0){
    pan_angle += 5;
  }

  //change tilt angle
  if (camera_commands[1] < 0){
    tilt_angle -= 5;
  }
  else if (camera_commands[1] > 0){
    tilt_angle += 5;
  }

  //command servo motors
  pan.write(pan_angle);
  tilt.write(tilt_angle);

  //read stationary flag toggle, change if needed
  if (digitalRead(STAT_TOGGLE_PIN) == HIGH){
    stationary_flag = !stationary_flag;
  }
  
  delay(10); //100hz task
}

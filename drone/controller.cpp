#include <math.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#include "controller.h"
#include "pid.h"

#define PGAIN_X 0.002
#define DGAIN_X 0.002

#define PGAIN_Y 0.002
#define DGAIN_Y 0.002

#define PGAIN_Z 0.002
#define DGAIN_Z 0.002

float[] get_hover_corrections
    (imu::Vector<3> orient, imu::Vector<3> gyro,
     float orient_ref[]){
    
    float orient_err[3];

    //X axis error
    orient_err[0] = orient.x - orient_ref[0];
    
    //Y axis error
    orient_err[1] = orient.y - orient_ref[1];
    
    //Z axis error
    orient_err[2] = orient.z - orient_ref[2];

    //Use PD controllers, derivative values are found in the sensor feed for acceleration

    float correction_torques[3];

    correction_torques[0] = pd(orient_err[0], gyro.x, PGAIN_X, DGAIN_X);
    correction_torques[1] = pd(orient_err[1], gyro.y, PGAIN_Y, DGAIN_Y);
    correction_torques[2] = pd(orient_err[2], gyro.z, PGAIN_Z, DGAIN_Z);

    return correction_torques;
}

float[] get_altitude_corrections(){

}

float[] get_command_corrections(){

}



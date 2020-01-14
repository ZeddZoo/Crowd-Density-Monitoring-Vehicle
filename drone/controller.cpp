#include <math.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#include "controller.h"

#define PGAIN_X 0.002
#define DGAIN_X 0.002

#define PGAIN_Y 0.002
#define DGAIN_Y 0.002

#define PGAIN_Z 0.002
#define DGAIN_Z 0.002

motor_torque_t get_hover_corrections
    (imu::Vector<3> orient, imu::Vector<3> gyro,
     float orient_ref[]){
    
    float orient_err[3];

    //X axis error
    orient_err[0] = orient.x - orient_ref[0];
    
    //Y axis error
    orient_err[1] = orient.y - orient_ref[1];
    
    //Z axis error
    orient_err[2] = orient.z - orient_ref[2];

    //PD controllers, derivative values are found in the
    //sensor feed for acceleration

    
    


    

}


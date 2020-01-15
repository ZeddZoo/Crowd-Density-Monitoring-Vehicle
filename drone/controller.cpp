#include <math.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#include "controller.h"
#include "pid.h"

/* 
 *
 * Below are configurations to tweak
 * All in SI units
 * 
 */


//PD controller gains
#define PGAIN_X 0.02
#define DGAIN_X 0.02

#define PGAIN_Y 0.02
#define DGAIN_Y 0.02

#define PGAIN_Z 0.02
#define DGAIN_Z 0.02

//motor parameters  (CHANGE LATER)
#define HOVER_TORQUE 20
#define MAX_TORQUE 50

//drone parameters (CHANGE LATER)
#define LENGTH 0.2
#define WIDTH 0.2
#define WEIGHT 0.4

/* 
 *
 * end config
 * 
 */

//begin functions

float pd(float p, float d, float pgain, float dgain){
    return (p * pgain) + (d * dgain);
}

//correct axes to match reference orientation
float *get_rotational_corrections
    (sensor_vec_t orient, imu::Vector<3> gyro,
     float orient_ref[]){
    
    float orient_err[3];

    //X axis error (roll)
    orient_err[0] = orient.x - orient_ref[0];
    
    //Y axis error (pitch)
    orient_err[1] = orient.y - orient_ref[1];
    
    //Z axis error (yaw)
    orient_err[2] = orient.z - orient_ref[2];

    //Use PD controllers, derivative values are found in the sensor feed for acceleration

    float correction_torques[3];

    //convert radians in gyro to degrees
    float rot_x = gyro.x() / 360.0 * 2.0 * 3.14159;
    float rot_y = gyro.y() / 360.0 * 2.0 * 3.14159;
    float rot_z = gyro.z() / 360.0 * 2.0 * 3.14159;

    correction_torques[0] = pd(orient_err[0], rot_x, PGAIN_X, DGAIN_X);
    correction_torques[1] = pd(orient_err[1], rot_y, PGAIN_Y, DGAIN_Y);
    correction_torques[2] = pd(orient_err[2], rot_z, PGAIN_Z, DGAIN_Z);

    return correction_torques;
}

//mix motors with base velocity for hover and bias to remove XYZ axis rotation errors, altitude bias
float mix_motors(float hover, float rot_corrections[], float hover_corrections[]) {

    float total_corrections[3];

    for (int i = 0; i < 3; i++){
        total_corrections[i] = rot_corrections[i] + alt_corrections;
    }

    for (int i = 0; i < 4; i++){
        base[i] = hover + hover_corrections[i];
    }

    float motor_torques[4];

    motor_torques[0] = base[0] + total_corrections[0] + total_corrections[1] + total_corrections[2];
    motor_torques[1] = base[1] - total_corrections[0] + total_corrections[1] - total_corrections[2];
    motor_torques[2] = base[2] + total_corrections[0] - total_corrections[1] - total_corrections[2];
    motor_torques[3] = base[3] - total_corrections[0] - total_corrections[1] + total_corrections[2];

    return motor_torques;
}



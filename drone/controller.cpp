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

float[] get_rotational_corrections
    (imu::Vector<3> orient, imu::Vector<3> gyro,
     float rotations[], float orient_ref[]){
    
    float orient_err[3];

    //X axis error (roll)
    orient_err[0] = orient.x - orient_ref[0];
    
    //Y axis error (pitch)
    orient_err[1] = orient.y - orient_ref[1];
    
    //Z axis error (yaw)
    orient_err[2] = orient.z - orient_ref[2];

    //Use PD controllers, derivative values are found in the sensor feed for acceleration

    float correction_torques[3];

    float rot_x = gyro.x / 360.0 * 2.0 * 3.14159;
    float rot_y = gyro.y / 360.0 * 2.0 * 3.14159;
    float rot_z = gyro.z / 360.0 * 2.0 * 3.14159;

    correction_torques[0] = pd(orient_err[0], rot_x, PGAIN_X, DGAIN_X);
    correction_torques[1] = pd(orient_err[1], rot_y, PGAIN_Y, DGAIN_Y);
    correction_torques[2] = pd(orient_err[2], rot_z, PGAIN_Z, DGAIN_Z);

    for (int i = 0; i < 3; i++){
        correction_torques[i] = rotations[i];
    }

    return correction_torques;
}

float get_hover_corrections(float command_accel, float sensor_accel, float hover){
    return pd(base - lin_accel, 0, 0.1, 0);
}

float mix_motors(float base, float rot_corrections[]) {

    float rot_corrections[3];
    float motor_torques[4];

    motor_torques[0] = base + rot_corrections[0] + rot_corrections[1] + rot_corrections[2];
    motor_torques[1] = base - rot_corrections[0] + rot_corrections[1] - rot_corrections[2];
    motor_torques[2] = base + rot_corrections[0] - rot_corrections[1] - rot_corrections[2];
    motor_torques[3] = base - rot_corrections[0] - rot_corrections[1] + rot_corrections[2];

    return motor_torques;
}



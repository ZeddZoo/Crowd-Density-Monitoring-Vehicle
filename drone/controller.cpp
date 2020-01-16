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
#define PGAIN_X 0.2
#define DGAIN_X 0.2

#define PGAIN_Y 0.2
#define DGAIN_Y 0.2

#define PGAIN_Z 0.2
#define DGAIN_Z 0.2

#define PGAIN_T -0.1

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
int *get_rotational_corrections
    (sensor_vec_t orient, imu::Vector<3> gyro,
     float orient_ref[]){
    
    float orient_err[3];

    //Angle overflow
    if (orient.x > 180.0){
        orient.x -= 360.0;
    }

    if (orient.y > 180.0){
        orient.y -= 360.0;
    }

    // if (orient.z > 180.0){
    //     orient.z -= 360.0;
    // }

    //X axis error (roll)
    orient_err[0] = orient.x - orient_ref[0];
    
    //Y axis error (pitch)
    orient_err[1] = orient.y - orient_ref[1];
    
    //Z axis error (yaw)
    orient_err[2] = orient.z - orient_ref[2];

    //Use PD controllers, derivative values are found in the sensor feed for acceleration

    int rot_corrections[3];

    //convert radians in gyro to degrees
    //gyro appears to be in degrees already

    // float rot_x = gyro.x() / 360.0 * 2.0 * 3.14159;
    // float rot_y = gyro.y() / 360.0 * 2.0 * 3.14159;
    // float rot_z = gyro.z() / 360.0 * 2.0 * 3.14159;

    float rot_x = gyro.x();
    float rot_y = gyro.y();
    float rot_z = gyro.z();

    rot_corrections[0] = (int)(pd(orient_err[0], rot_x, PGAIN_X, DGAIN_X));
    rot_corrections[1] = (int)(pd(orient_err[1], rot_y, PGAIN_Y, DGAIN_Y));
    rot_corrections[2] = (int)(pd(orient_err[2], rot_z, PGAIN_Z, DGAIN_Z));

    return rot_corrections;
}

int get_hover_corrections(float vz, float vz_ref){
    return (vz - vz_ref) * PGAIN_T;
}

//mix motors with base velocity for hover and bias to remove XYZ axis rotation errors, altitude bias

//TODO: calibrate values

float mix_motors(int hover, int rot_corrections[], int hover_corrections) {

    int base = hover + hover_corrections;

    int motor_torques[4];

    motor_torques[0] = base + rot_corrections[0] + rot_corrections[1] + rot_corrections[2];
    motor_torques[1] = base - rot_corrections[0] + rot_corrections[1] - rot_corrections[2];
    motor_torques[2] = base + rot_corrections[0] - rot_corrections[1] - rot_corrections[2];
    motor_torques[3] = base - rot_corrections[0] - rot_corrections[1] + rot_corrections[2];

    return motor_torques;
}



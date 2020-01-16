#include <algorithm> 
#include "controller.h"

#define MAX_X_ANGLE 10.0
#define MAX_Y_ANGLE 10.0
#define MAX_Z_ANGLE 10.0

#define VZ_GAIN -1.0
#define ORI_GAIN 1.0

float *xyz_to_orient_ref(int commands[]){
    float orient_ref[3];

    orient_ref[0] = commands[0] * MAX_X_ANGLE;
    orient_ref[1] = commands[1] * MAX_Y_ANGLE;
    orient_ref[2] = commands[2] * MAX_Z_ANGLE;

    return orient_ref;
}

float get_reference_vz(int commands[]){
    float vz = commands[3] * VZ_GAIN;
    return vz;
}

float *cancel_translational_movement(float vx, float vy, float vz){
    float orient_ref[3] = {0.0, 0.0, 0.0};
    
    orient_ref[0] = std::max(MAX_X_ANGLE, vx * ORI_GAIN);
    orient_ref[1] = std::max(MAX_Y_ANGLE, vy * ORI_GAIN);
    orient_ref[2] = std::max(MAX_Z_ANGLE, vz * ORI_GAIN);

    return orient_ref;
}
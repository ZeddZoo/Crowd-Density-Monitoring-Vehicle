#define MAX_X_ANGLE 10
#define MAX_Y_ANGLE 10
#define MAX_Z_ANGLE 10

float *translate_commands_to_orient_ref(int commands[]){
    float orient_ref[3];

    orient_ref[0] = commands[0] * MAX_X_ANGLE;
    orient_ref[1] = commands[1] * MAX_Y_ANGLE;
    orient_ref[2] = commands[2] * MAX_Z_ANGLE;

    return orient_ref;

}

float *translate_commands_to_hover_corrections(int commands[]){
    
}
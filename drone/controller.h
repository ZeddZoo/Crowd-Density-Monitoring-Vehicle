float pd(float p, float d, float pgain, float dgain);

float[] get_rotational_corrections
    (imu::Vector<3> orient, imu::Vector<3> gyro,
     float rotations[], float orient_ref[]);

float get_hover_corrections(float command_accel, float sensor_accel, float hover);

float mix_motors(float base, float rot_corrections[]);


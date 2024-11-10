#pragma once

#include <inttypes.h>

typedef struct {
  float kp;
  float ki;
  float kd;
  float prev_error;
  float integral;
} PI_controller;

float PI_controller_update(PI_controller* controller, float measurement,float setpoint);

void PI_controller_reset(PI_controller* controller);
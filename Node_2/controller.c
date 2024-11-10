#include "controller.h"

float PI_controller_update(PI_controller* controller, float measurement,float setpoint)
{
  float e = setpoint - measurement;

  controller->integral = controller->integral + e;


  if(controller->integral > 200){controller->integral = 200;}
  if(controller->integral < -200){controller->integral = -200;}

  float u = (controller->kp * e) +(controller->ki * controller->integral) + controller->kd * (e - controller->prev_error);

  controller->prev_error= e;

  if(u > 127){u = 127;}
  if(u < -128){u = -128;}

  return u; 
}

void PI_controller_reset(PI_controller* controller)
{
  controller->integral = 0;
  controller->prev_error = 0;
}

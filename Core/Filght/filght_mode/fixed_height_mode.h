#ifndef _HAL_FIXED_HEIGHT_MODE_
#define _HAL_FIXED_HEIGHT_MODE_

#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "state.h"
#include "config.h"
#include "motor_control.h"
#include "pid.h"
#include "usb.h"

#include "filght_mode.h"

  typedef struct Fixed_Height_Mode
  {
    state_t *active;
    state_t *state;
  } fixed_h_m_t;

  fixed_h_m_t *New_Fixed_Height_Mode(filght_mode_t *ctx);

  extern pid_t fixed_height_pid;
  extern volatile pid_c_t pid_value;

#ifdef __cplusplus
}
#endif
#endif
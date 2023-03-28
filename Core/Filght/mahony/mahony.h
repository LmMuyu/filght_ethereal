#ifndef _HAL_MAHONY_
#define _HAL_MAHONY_
#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "flight_math.h"
#include "math.h"
#include "stdio.h"

#define Kpv (2.0f * 0.5f)
#define Kiv (2.0f * 0.5f)

#define Seep 1000.0f

  extern volatile float Kp;
  extern volatile float Ki;

  float *MahonyAttitude(float gx, float gy, float gz, float ax, float ay, float az);

  float *EulerAngle(float q[4]);

  float Module_Length();

#ifdef __cplusplus
}
#endif
#endif
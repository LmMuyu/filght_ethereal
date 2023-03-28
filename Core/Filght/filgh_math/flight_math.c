#include "flight_math.h"

float Filght_Sqrt(float x)
{

  float xhalf = 0.5f * x;
  int i = *(int *)&x;             // get bits for floating VALUE
  i = 0x5f375a86 - (i >> 1);      // gives initial guess y0
  x = *(float *)&i;               // convert bits BACK to float
  x = x * (1.5f - xhalf * x * x); // Newton step, repeating increases accuracy
  return x;
}

float Filght_square(float x)
{
  return x * x;
}
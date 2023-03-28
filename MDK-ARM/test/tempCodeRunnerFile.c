#include "stdio.h"
#include "string.h"
#include "stdint.h"

union
{
  float fa;
  char farray[4];
} utemp;

typedef struct MOTOR
{
  float f_v;
} motor_t;

motor_t pdata[4] = {{123.8}};

int main(int argc, char const *argv[])
{
  pdata[0].f_v = 12.9;

  printf("%f\n", pdata[0].f_v);
  return 0;
}

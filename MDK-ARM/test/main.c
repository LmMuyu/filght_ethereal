#include "stdio.h"

typedef struct MOTOR
{
  float f_v;
} motor_t;

typedef union Motor_Dist_Value
{
  float M1;
  float M2;
  float M3;
  float M4;
  char Mx[16];
} motor_dist_value_t;

motor_t pdata[4];

motor_dist_value_t motor_d = {
    .M1 = 92.3,
    .M2 = 32.3,
    .M3 = 22.3,
    .M4 = 92.3,
};

int main(int argc, char const *argv[])
{

  printf("%f\n", *(float *)&motor_d.Mx[0]);
  printf("%f\n", *(float *)&motor_d.Mx[4]);
  printf("%f\n", *(float *)&motor_d.Mx[8]);
  printf("%f\n", *(float *)&motor_d.Mx[12]);

  while (1)
  {
    /* code */
  }

  return 0;
}

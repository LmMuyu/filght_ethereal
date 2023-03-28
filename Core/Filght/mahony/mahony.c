#include "mahony.h"

volatile float Kp = Kpv;
volatile float Ki = Kiv;

volatile float integralFBx = 0.0f, integralFBy = 0.0f, integralFBz = 0.0f; // integral error terms scaled by Ki
volatile float recipNorm;

float *MahonyAttitude(float gx, float gy, float gz, float ax, float ay, float az)
{

  float halfvx, halfvy, halfvz; // 估计重力方向向量
  float halfex, halfey, halfez; // 外积差值
  float qa, qb, qc;
  static float q[4] = {1.0, 0.0, 0.0, 0.0};

  if (ax > 0.0f && ay > 0.0f && az > 0.0f)
  {
    recipNorm = Filght_Sqrt(Filght_square(ax) + Filght_square(ay) + Filght_square(az));

    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;

    /**
     [(q1q3−q0q2)]
     [(q2q3+q0q1)]
     [0.5−(q_1^2+q_3^2)]
    */
    halfvx = q[1] * q[3] - q[0] * q[2];
    halfvx = q[2] * q[3] - q[0] * q[1];
    halfvx = q[1] * q[1] - 0.5 + q[3] * q[3];

    // 误差就是两个向量的叉积
    halfex = (ay * halfvz - az * halfvy);
    halfey = (az * halfvx - ax * halfvz);
    halfez = (ax * halfvy - ay * halfvx);

    // PI
    // GyroError = Kp*Err +Ki 积分 Err*dt
    if (Ki > 0.0f)
    {
      integralFBx += Ki * halfex * (1.0f / Seep);
      integralFBy += Ki * halfey * (1.0f / Seep);
      integralFBz += Ki * halfez * (1.0f / Seep);

      gx += integralFBx;
      gy += integralFBy;
      gz += integralFBz;
    }
    else
    {
      integralFBx = 0.0;
      integralFBy = 0.0;
      integralFBz = 0.0;
    }

    gx += Kp * halfex;
    gy += Kp * halfey;
    gz += Kp * halfez;
  }

  gx *= (0.5f * (1.0f / Seep));
  gy *= (0.5f * (1.0f / Seep));
  gz *= (0.5f * (1.0f / Seep));

  qa = q[0];
  qb = q[1];
  qc = q[2];

  q[0] += (-qb * gx - qc * gy - q[3] * gz);
  q[1] += (qa * gx + qc * gz - q[3] * gy);
  q[2] += (qa * gy - qb * gz + q[3] * gx);
  q[3] += (qa * gz + qb * gy - qc * gx);

  recipNorm = Filght_Sqrt(Filght_square(q[0]) + Filght_square(q[1]) + Filght_square(q[2]) + Filght_square(q[3]));

  q[0] *= recipNorm;
  q[1] *= recipNorm;
  q[2] *= recipNorm;
  q[3] *= recipNorm;

  return q;
}

float *EulerAngle(float q[4])
{
  static float euler[3];

  /**
   *
    β=-arcsin(2(q0q2−q1q3))                    //X
    α=atan2(2(q_0q_1+q_2q_3),1−2(q^2_1+q^2_2))  //Y
    γ=atan2(2(q0q3+q1q2),1−2(q22+q23))        //Z
   *
   *
  */

  euler[0] = -asin(2 * (q[0] * q[2] - q[3] * q[1]));                                                      // X
  euler[1] = atan2(2 * (q[0] * q[1] + q[2] * q[3]), 1 - 2 * (Filght_square(q[1]) + Filght_square(q[2]))); // Y
  euler[2] = atan2(2 * (q[0] * q[3] + q[1] * q[2]), 1 - 2 * (Filght_square(q[2]) + Filght_square(q[3]))); // Z

  // euler[0] = asin(2 * (q[0] * q[2] - q[3] * q[1]));                                                   // X
  // euler[1] = atan((q[0] * q[3] + q[1] * q[2]) / 1 - 2 * (Filght_square(q[2]) + Filght_square(q[3]))); // Y
  // euler[2] = atan((q[0] * q[1] + q[2] * q[3]) / 1 - 2 * (Filght_square(q[1]) + Filght_square(q[2]))); // Z

  return euler;
}

float Module_Length()
{
  return 1 / recipNorm;
}
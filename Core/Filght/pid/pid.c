#include "pid.h"

float Pid_Positional_Type(pid_t *pid_example, pid_c_t *pid_codf)
{
  float err = pid_example->target_value - pid_example->watch_v;
  float diff_last_err = err - pid_example->last_v;

  float Pid_Pout = pid_codf->P * err;                      // 比例误差
  pid_example->int_err_sup += pid_codf->I * diff_last_err; // 积分叠加
  float Pid_Dout = pid_codf->D * diff_last_err;            // 微分修正

  // 积分项饱和,超过了最大值，将他回退到指定最大值
  if (pid_example->int_err_sup > pid_example->int_max)
    pid_example->int_err_sup = pid_example->int_max;

  pid_example->last_v = err;

  if (pid_example->watch_count >= 2)
    pid_example->last_v2 = pid_example->last_v;

  pid_example->watch_count += 1;

  return Pid_Pout + pid_example->int_err_sup + Pid_Dout;
}

float Pid_Incremental(pid_t *pid_example, pid_c_t *pid_codf)
{
  /**
    比例P : e(k)-e(k-1) 这次误差-上次误差

    积分I : e(i) 误差

    微分D : e(k) - 2e(k-1)+e(k-2) 这次误差-2*上次误差+上上次误差
  */

  float err = pid_example->target_value - pid_example->watch_v;
  float diff_last_err = err - pid_example->last_v;

  float Pid_Pout = pid_codf->P * diff_last_err;            // 比例误差
  pid_example->int_err_sup += pid_codf->I * diff_last_err; // 积分叠加

  if (pid_example->int_err_sup > pid_example->int_max)
    pid_example->int_err_sup = pid_example->int_max;

  if (pid_example->watch_count >= 2)
    pid_example->last_v2 = pid_example->last_v;

  float Pid_Dout;

  if (pid_example->watch_count >= 2)
  {
    Pid_Dout = pid_codf->D * (diff_last_err - 2 * (pid_example->last_v) + pid_example->watch_count >= 2 ? pid_example->last_v2 : 0); // 微分修正
  }

  pid_example->last_v = err;

  pid_example->watch_count += 1;

  return Pid_Pout + pid_example->int_err_sup + Pid_Dout;
}
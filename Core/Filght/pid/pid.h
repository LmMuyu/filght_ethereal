#ifndef _HAL_PID_
#define _HAL_PID_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "stdbool.h"

  typedef struct PID
  {
    float target_value;   // 期待值
    float int_max;        // 积分最大值
    float last_v;         // 上一次误差
    float last_v2;        // 上二次误差
    float int_err_sup;    // 积分误差叠加
    float watch_v;        // 观测值
    uint32_t watch_count; // 计算次数
  } pid_t;

  typedef struct PID_Coefficient
  {
    float P;
    float I;
    float D;
  } pid_c_t;

  /// @brief 位置式PID
  /// @param pid_example
  /// @param pid_codf
  /// @return PID预期量
  float Pid_Positional_Type(pid_t *pid_example, pid_c_t *pid_codf);

  /// @brief 增量式PID
  /// @param pid_example
  /// @param pid_codf
  /// @return PID预期量
  float Pid_Incremental(pid_t *pid_example, pid_c_t *pid_codf);

#ifdef __cplusplus
}
#endif
#endif
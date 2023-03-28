#include "mpu6050.h"

const char *const Task_Name = "mpu6050_task";
i2c_instance_methods_t *hi2c_example;
TaskHandle_t xHandle;
StaticTask_t xTaskBufType;
StackType_t xBufferStack[STACK_SIZE];

bool calibration = false;
uint32_t cali_count = 100;
float gyro_offset[3];
float accle_offset[3];

mpu6050_type_t mpu_instance;

void Mpu_Reg_Init(void);
HAL_StatusTypeDef Mpu_Reg_Write(uint8_t reg, uint8_t value);
void Mpu_Gyroscope_Read(uint8_t reg);
void Mpu_Accle_Read(uint8_t reg);
void Mpu6050_Task_Function(void *args);
void Mpu_Fusion_Processing();

void Mpu6050_Task_Handle(void)
{
  xHandle = xTaskCreateStatic(Mpu6050_Task_Function, Task_Name, STACK_SIZE, NULL, Task_Priority_Level_10, xBufferStack, &xTaskBufType);
}

void Mpu6050_Task_Function(void *parameTask)
{
  hi2c_example = I2c_CreateMethods();

  if ((hi2c_example->i2c_example->hi2c->Instance == I2C1))
  {
    Mpu_Reg_Init();
    mpu_instance.accle = (accel_type_t *)malloc(sizeof(accel_type_t));
    mpu_instance.gyro = (gyro_type_t *)malloc(sizeof(gyro_type_t));

    while (1)
    {
      Mpu_Gyroscope_Read(GYRO_XOUT_H);
      Mpu_Accle_Read(ACCEL_XOUT_H);
      Mpu_Fusion_Processing();

      vTaskDelay(pdMS_TO_TICKS(10));
    }
  }
}

void Mpu_Reg_Init(void)
{
  Mpu_Reg_Write(PWR_MGMT_1_Reg, PWR_MGMT); // 电源管理1
  Mpu_Reg_Write(Freq_Reg, Freq);           // 频率寄存器
  Mpu_Reg_Write(Filter_Reg, Filter);       // 过滤器地址
  Mpu_Reg_Write(Gcfg_Reg, Gcfg_V);         // 陀螺仪配置
  Mpu_Reg_Write(Acfg_Reg, Acfg_V);         // 加速度配置
}

HAL_StatusTypeDef Mpu_Reg_Write(uint8_t reg, uint8_t value)
{
  return I2c_WriteData((uint16_t)(MPU6050_ADDR), reg, &value, 1);
}

void Mpu_Gyroscope_Read(uint8_t reg)
{
  I2c_Mem_ReadData((uint16_t)(MPU6050_ADDR), reg, mpu_instance.accle->accel_lists, 6);

  int16_t gx = (int16_t)(mpu_instance.accle->accel_lists[0] << 8) | mpu_instance.accle->accel_lists[1];
  int16_t gy = (int16_t)(mpu_instance.accle->accel_lists[2] << 8) | mpu_instance.accle->accel_lists[3];
  int16_t gz = (int16_t)(mpu_instance.accle->accel_lists[4] << 8) | mpu_instance.accle->accel_lists[5];

  mpu_instance.gyro->gx += gx / GYRO_LSB;
  mpu_instance.gyro->gy += gy / GYRO_LSB;
  mpu_instance.gyro->gz += gz / GYRO_LSB;
}

void Mpu_Accle_Read(uint8_t reg)
{
  I2c_Mem_ReadData((uint16_t)(MPU6050_ADDR), reg, mpu_instance.accle->accel_lists, 6);

  int16_t ax = (int16_t)(mpu_instance.accle->accel_lists[0] << 8) | mpu_instance.accle->accel_lists[1];
  int16_t ay = (int16_t)(mpu_instance.accle->accel_lists[2] << 8) | mpu_instance.accle->accel_lists[3];
  int16_t az = (int16_t)(mpu_instance.accle->accel_lists[4] << 8) | mpu_instance.accle->accel_lists[5];

  mpu_instance.accle->ax += ax / ACCLE_LSB;
  mpu_instance.accle->ay += ay / ACCLE_LSB;
  mpu_instance.accle->az += az / ACCLE_LSB;
}

void Mpu_Fusion_Processing()
{
  if (!calibration)
    cali_count -= 1;

  if (cali_count <= 0 && !calibration)
  {
    gyro_offset[0] = mpu_instance.gyro->gx / 100;
    gyro_offset[1] = mpu_instance.gyro->gy / 100;
    gyro_offset[2] = mpu_instance.gyro->gz / 100;

    accle_offset[0] = mpu_instance.accle->ax / 100;
    accle_offset[1] = mpu_instance.accle->ay / 100;
    accle_offset[2] = (mpu_instance.accle->az - 9.8) / 100;

    mpu_instance.gyro->gz = mpu_instance.gyro->gy = mpu_instance.gyro->gx = 0.0;
    mpu_instance.accle->ax = mpu_instance.accle->ay = mpu_instance.accle->az = 0.0;
    calibration = true;
  }

  if (calibration)
  {
    mpu_instance.gyro->gx -= gyro_offset[0];
    mpu_instance.gyro->gy -= gyro_offset[1];
    mpu_instance.gyro->gz -= gyro_offset[2];

    mpu_instance.accle->ax -= accle_offset[0];
    mpu_instance.accle->ay -= accle_offset[1];
    mpu_instance.accle->az -= accle_offset[2];

    float *q = MahonyAttitude(mpu_instance.gyro->gx, mpu_instance.gyro->gy, mpu_instance.gyro->gz, mpu_instance.accle->ax, mpu_instance.accle->ay, mpu_instance.accle->az);
    float *euler = EulerAngle(q);

    expected_thrust_t thrust = {
        .az = mpu_instance.accle->az,
    };

    thrust.gravity_g = Module_Length();

    Motor_Control_Distribution(euler, &thrust);

    mpu_instance.gyro->gz = mpu_instance.gyro->gy = mpu_instance.gyro->gx = 0.0;
    mpu_instance.accle->ax = mpu_instance.accle->ay = mpu_instance.accle->az = 0.0;

    // Vofa_Just_Float(q);

    free(q);
    free(euler);
  }
}
#include "hal_i2c.h"

volatile uint8_t i2cIndex = 1;
i2c_instance_methods_t *hi2c_instance_mts;
I2C_HandleTypeDef hi2c;


void HAL_LOGE(char *errormsg, HAL_StatusTypeDef actualStatus, HAL_StatusTypeDef correctStatus);

/// @brief
/// @param actualStatus 返回的状态
/// @param correctStatus 正确的状态
void I2c_Request(HAL_I2C_StateTypeDef actualStatus, HAL_I2C_StateTypeDef correctStatus);

void I2c_Port_Init();

i2c_instance_methods_t *I2c_CreateMethods(void)
{
  if (HAL_I2C_GetState(&hi2c) != HAL_I2C_STATE_RESET)
    return hi2c_instance_mts;

  I2c_Init();

  static i2c_example_t i2c_ex = {
      .hi2c = &hi2c,
      .multiple = Multiple,
  };

  static i2c_instance_methods_t hi2c_mts = {
      .i2c_example = &i2c_ex,
      .I2c_Mem_ReadData = I2c_Mem_ReadData,
      .I2c_ReadData = I2c_ReadData,
      .I2c_WriteData = I2c_WriteData,
      .I2c_Port_DeInit = I2c_Port_DeInit,
  };

  hi2c_instance_mts = &hi2c_mts;

  return hi2c_instance_mts;
}

void I2c_Init(void)
{
  if (HAL_I2C_GetState(&hi2c) == HAL_I2C_STATE_RESET)
  {
    __HAL_RCC_I2C1_CLK_ENABLE();

    hi2c.Instance = I2C1;
    hi2c.Init.Timing = I2C1_200khz;
    hi2c.Init.OwnAddress1 = 0;
    hi2c.Init.OwnAddress2 = 0;
    hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;

    HAL_LOGE("HAL_I2C_Init", HAL_I2C_Init(&hi2c), HAL_OK);

    // // 数字噪声过滤器
    // HAL_LOGE("HAL_I2CEx_ConfigDigitalFilter", HAL_I2CEx_ConfigDigitalFilter(&hi2c, 0x00), HAL_OK);

    I2c_Port_Init();
  }
}

void I2c_Port_Init()
{
  if (hi2c.Instance == I2C1)
  {
    GPIO_InitTypeDef I2c_Port = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    I2c_Port.Pin = I2C_SCL | I2C_SDA;
    I2c_Port.Mode = GPIO_MODE_AF_PP;
    I2c_Port.Speed = GPIO_SPEED_FREQ_HIGH;
    I2c_Port.Pull = GPIO_NOPULL;
    I2c_Port.Alternate = GPIO_AF4_I2C1;

    HAL_GPIO_Init(I2C_REG, &I2c_Port);
  }
}

HAL_StatusTypeDef I2c_WriteData(uint16_t slave_write_addr, uint8_t slave_reg, uint8_t *data, uint16_t len)
{
  int Timeout;
  slave_write_addr <<= 1;
  Timeout = Tx_Timeout;

  while (HAL_I2C_GetState(hi2c_instance_mts->i2c_example->hi2c) == HAL_I2C_STATE_BUSY)
  {
    if ((Timeout -= 1) <= 0)
    {
      Hal_Write_Buf("HAL_BUSY\r\n");
      Hal_SendData();

      return HAL_BUSY;
    }
  }

  if (hi2c_instance_mts->i2c_example->multiple == NMultiple)
    HAL_I2C_Master_Transmit(hi2c_instance_mts->i2c_example->hi2c, slave_write_addr, data, 1, Tx_Timeout);
  else
  {
    HAL_I2C_Mem_Write(hi2c_instance_mts->i2c_example->hi2c, slave_write_addr, slave_reg, 1, data, len, Tx_Timeout);
  }

  Timeout = Tx_Timeout;
  while (HAL_I2C_GetState(hi2c_instance_mts->i2c_example->hi2c) == HAL_I2C_STATE_TIMEOUT)
  {
    if ((Timeout -= 1) <= 0)
    {
      Hal_Write_Buf("HAL_TIMEOUT\r\n");
      Hal_SendData();
      return HAL_TIMEOUT;
    }
  }

  i2cIndex = 1;

  return HAL_OK;
}

HAL_StatusTypeDef I2c_ReadData(uint16_t slave_read_addr, uint8_t *recv_buf, uint16_t len)
{
  I2c_Request(HAL_I2C_GetState(hi2c_instance_mts->i2c_example->hi2c), HAL_I2C_STATE_BUSY);
  HAL_I2C_Master_Receive(hi2c_instance_mts->i2c_example->hi2c, (sizeof(slave_read_addr) == 2) ? slave_read_addr : (uint8_t)(slave_read_addr), recv_buf, len, Rx_Timeout);
  I2c_Request(HAL_I2C_GetState(hi2c_instance_mts->i2c_example->hi2c), HAL_I2C_STATE_TIMEOUT);

  i2cIndex = 1;

  return HAL_OK;
}

HAL_StatusTypeDef I2c_Mem_ReadData(
    uint8_t slave_addr,
    uint8_t mem_addr,
    uint8_t *recv_buf,
    uint16_t readlen)
{

  int Timeout = Tx_Timeout;
  slave_addr = ((slave_addr << 1) + 1);

  while (HAL_I2C_GetState(hi2c_instance_mts->i2c_example->hi2c) == HAL_I2C_STATE_BUSY)
  {
    if ((Timeout -= 1) <= 0)
    {
      Hal_Write_Buf("HAL_BUSY\r\n");
      Hal_SendData();
      return HAL_BUSY;
    }
  }

  if (hi2c_instance_mts->i2c_example->multiple == NMultiple)
    HAL_I2C_Master_Receive(hi2c_instance_mts->i2c_example->hi2c, slave_addr, recv_buf, 1, Rx_Timeout);
  else
    HAL_I2C_Mem_Read(hi2c_instance_mts->i2c_example->hi2c, slave_addr, mem_addr, I2C_MEMADD_SIZE_8BIT, recv_buf, readlen, Rx_Timeout);

  return HAL_OK;
}

void I2c_Port_DeInit()
{
  if (hi2c_instance_mts->i2c_example->hi2c->Instance == I2C1)
  {
    __HAL_RCC_GPIOB_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOB, I2C_SCL);
    HAL_GPIO_DeInit(GPIOB, I2C_SDA);
  }
}

void HAL_LOGE(char *errormsg, HAL_StatusTypeDef actualStatus, HAL_StatusTypeDef correctStatus)
{

  if (actualStatus != correctStatus)
  {

    char sendmes[128];
    sprintf(sendmes, "Function Error： %s", errormsg);
    Hal_Write_Buf(sendmes);

    while (1)
    {
      Hal_SendData();
      HAL_Delay(1000);
    }
  }
}

void I2c_Request(HAL_I2C_StateTypeDef actualStatus, HAL_I2C_StateTypeDef correctStatus)
{

  if (actualStatus == correctStatus)
  {
    char statuserrormsg[256];
    sprintf(statuserrormsg, "%d.%d\r\n", i2cIndex, (uint8_t)actualStatus);

    Hal_Write_Buf(statuserrormsg);

    while (1)
    {
      Hal_SendData();
      HAL_Delay(1000);
    }
  };

  i2cIndex++;
}

uint8_t *Assem_Pdata(const int hi2c_reg_data, ...)
{
  va_list i2c_vp;
  va_start(i2c_vp, hi2c_reg_data);

  int len = sizeof(hi2c_reg_data);
  static uint8_t *pdata;

  pdata = (uint8_t *)calloc(1, sizeof(uint8_t));
  pdata[0] = hi2c_reg_data;

  for (size_t i = 1; i < len - 1; i++)
  {
    pdata[i] = va_arg(i2c_vp, int);
  }

  va_end(i2c_vp);

  Hal_Write_Buf((char *)pdata);
  Hal_SendData();

  return pdata;
}

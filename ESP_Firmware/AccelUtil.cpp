#include <Wire.h>
#include <stdint.h>
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include "AccelUtil.h"

uint16_t accelDataX;
uint16_t accelDataY;
uint16_t accelDataZ;
uint16_t gyroDataX;
uint16_t gyroDataY;
uint16_t gyroDataZ;

void readData() {


  //Define pointer Buffers

  uint8_t* bufferptr1 = (uint8_t*) accelDataX;
  uint8_t* bufferptr2 = (uint8_t*) accelDataY;
  uint8_t* bufferptr3 = (uint8_t*) accelDataZ;
  uint8_t* bufferptr4 = (uint8_t*) gyroDataX;
  uint8_t* bufferptr5 = (uint8_t*) gyroDataY;
  uint8_t* bufferptr6 = (uint8_t*) gyroDataZ;

  // Read most recent measurements  from 12 registers
  // containing acceleration and gyro Data
  // With the default settings of the MPU-6050, Most registers 0x00
  //From setupMPU() the frequency of sampling is 8kHz
  //MPUread(MPU6050_TEMP_OUT_H, uint8_t accelData.x_high, sizeof(accelData.x_high));
  //MPUread(MPU6050_TEMP_OUT_L, uint8_t accelData.x_high, sizeof(accelData.x_high));

  MPUread(MPU6050_ACCEL_XOUT_H, &bufferptr1[0], 1);
  MPUread(MPU6050_ACCEL_XOUT_L, &bufferptr1[1], 1);
  MPUread(MPU6050_ACCEL_YOUT_H, &bufferptr2[0], 1);
  MPUread(MPU6050_ACCEL_YOUT_L, &bufferptr2[1], 1);
  MPUread(MPU6050_ACCEL_ZOUT_H, &bufferptr3[0], 1);
  MPUread(MPU6050_ACCEL_ZOUT_L, &bufferptr3[1], 1);
  MPUread(MPU6050_GYRO_XOUT_H, &bufferptr4[0], 1);
  MPUread(MPU6050_GYRO_XOUT_L, &bufferptr4[1], 1);
  MPUread(MPU6050_GYRO_YOUT_H, &bufferptr5[0], 1);
  MPUread(MPU6050_GYRO_YOUT_L, &bufferptr5[1], 1);
  MPUread(MPU6050_GYRO_ZOUT_H, &bufferptr6[0], 1);
  MPUread(MPU6050_GYRO_ZOUT_L, &bufferptr6[1], 1);


 // accelDataX[0] = bufferptr1[0]+bufferptr1[1];
 // accelDataY[0] = bufferptr2[0]+bufferptr2[1];
 // accelDataZ[0] = bufferptr3[0]+bufferptr3[1];
 // gyroDataX[0] = bufferptr4[0]+bufferptr4[1];
 // gyroDataY[0] = bufferptr5[0]+bufferptr5[1];
 // gyroDataZ[0] = bufferptr6[0]+bufferptr6[1];

}

int MPUread(uint16_t start, uint8_t *buffer, uint16_t sizeAdd)
{
  uint8_t inputByte;

  Wire.beginTransmission(MPUAddress);
  Wire.write(start);
  Wire.endTransmission(false);

  Wire.requestFrom(MPUAddress, sizeAdd, true);

  while (Wire.available())
  {
    inputByte = 0xFF & Wire.read();
    *buffer = inputByte;
  }
}

int MPUwrite(uint16_t start, const uint8_t *pData, uint16_t sizeAdd)
{

  Wire.beginTransmission(MPUAddress);
  Wire.write(start);        // write the start address
  Wire.write(pData, sizeAdd);  // write data bytes
  Wire.endTransmission(true); // release the I2C-bus

  return (0);         // return : no error
}

//Single register write
int MPUwrite_reg(uint16_t reg, uint8_t data)
{
  MPUwrite(reg, &data, 1);

}

void setupMPU() {

  //Function to set MPU registers
  //Set SampleRate Divider to 1/8 or 1000Hz
  MPUwrite_reg(MPU6050_SMPLRT_DIV, 0x07);
  //Disable FSync and DLPF (Digital LowPass Filter)
  //DLPF set to 00000100
  //1kHz sample period
  //8.5 and 8.3 delays for accel and gyro resp.
  MPUwrite_reg(MPU6050_CONFIG, 0x00);
  //Disable Gyro Self Tests, set range +-500 deg/sec
  MPUwrite_reg(MPU6050_GYRO_CONFIG, 0x07);//11101000
  //Disable Accel Self Tests, Set range to +-4g
  MPUwrite_reg(MPU6050_ACCEL_CONFIG, 0x07);

  //Disable a bunch of stuff
  MPUwrite_reg( MPU6050_FF_THR, 0x00);
  MPUwrite_reg(MPU6050_FF_DUR, 0x00);
  MPUwrite_reg(MPU6050_MOT_THR, 0x00);
  MPUwrite_reg(MPU6050_MOT_DUR, 0x00);
  MPUwrite_reg(MPU6050_ZRMOT_THR, 0x00);
  MPUwrite_reg(MPU6050_ZRMOT_DUR, 0x00);
  MPUwrite_reg(MPU6050_FIFO_EN, 0x00);
  MPUwrite_reg(MPU6050_ZRMOT_DUR, 0x00);
  MPUwrite_reg(MPU6050_ZRMOT_DUR, 0x00);
  MPUwrite_reg(MPU6050_ZRMOT_DUR, 0x00);

  //Master Control set to Zero
  MPUwrite_reg(MPU6050_I2C_MST_CTRL, 0x00);
  //Setup AUX I2C Slaves
  MPUwrite_reg(MPU6050_I2C_SLV0_ADDR, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV0_REG, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV0_CTRL, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV1_ADDR, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV1_REG, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV1_CTRL, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV2_ADDR, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV2_REG, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV2_CTRL, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV3_ADDR, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV3_REG, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV3_CTRL, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV4_ADDR, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV4_REG, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV4_DO, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV4_CTRL, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV4_DI, 0x00);
  MPUwrite_reg(MPU6050_I2C_MST_STATUS, 0x00);
  MPUwrite_reg(MPU6050_INT_PIN_CFG, 0x00);
  MPUwrite_reg(MPU6050_INT_ENABLE, 0x00);

  //READ ONLY REGISTERS
  /*
  MPUread(MPU6050_INT_STATUS, 0x00);
   MPUread(MPU6050_ACCEL_XOUT_H, 0x00);
   MPUread(MPU6050_ACCEL_XOUT_L, 0x00);
   MPUread(MPU6050_ACCEL_YOUT_H, 0x00);
   MPUread(MPU6050_ACCEL_YOUT_L, 0x00);
   MPUread(MPU6050_ACCEL_ZOUT_H, 0x00);
   MPUread(MPU6050_ACCEL_ZOUT_L, 0x00);
   MPUread(MPU6050_TEMP_OUT_H, 0x00);
   MPUread(MPU6050_TEMP_OUT_L, 0x00);
   MPUread(MPU6050_GYRO_XOUT_H, 0x00);
   MPUread(MPU6050_GYRO_XOUT_L, 0x00);
   MPUread(MPU6050_GYRO_YOUT_H, 0x00);
   MPUread(MPU6050_GYRO_YOUT_L, 0x00);
   MPUread(MPU6050_GYRO_ZOUT_H, 0x00);
   MPUread(MPU6050_GYRO_ZOUT_L, 0x00);

   */

  //Slave Data out and on
  MPUwrite_reg(MPU6050_I2C_SLV0_DO, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV1_DO, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV2_DO, 0x00);
  MPUwrite_reg(MPU6050_I2C_SLV3_DO, 0x00);
  MPUwrite_reg(MPU6050_I2C_MST_DELAY_CTRL, 0x00);
  MPUwrite_reg(MPU6050_SIGNAL_PATH_RESET, 0x00);
  MPUwrite_reg(MPU6050_MOT_DETECT_CTRL, 0x00);
  MPUwrite_reg(MPU6050_USER_CTRL, 0x00);
  MPUwrite_reg(MPU6050_PWR_MGMT_1, 0x00);
  MPUwrite_reg(MPU6050_PWR_MGMT_2, 0x00);
  MPUwrite_reg(MPU6050_FIFO_COUNTH, 0x00);
  MPUwrite_reg(MPU6050_FIFO_COUNTL, 0x00);
  MPUwrite_reg(MPU6050_FIFO_R_W, 0x00);

	//return true;
}

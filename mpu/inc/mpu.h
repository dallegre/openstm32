#include "mpu_defines.h"

int read_i2c(uint8_t address);
void write_i2c(uint8_t address, uint8_t data);

int verify_i2c(void){

  int data = read_i2c(MPU6050_RA_WHO_AM_I);
  int data2 = read_i2c(MPU6050_RA_PWR_MGMT_1);

  if(data == 0x68 && data2 == 0x40){
	  return 1;
  }else{
	  return 0;
  }

}

int check_sleep(void){
	  int data = read_i2c(MPU6050_RA_PWR_MGMT_1);
	  if(data == 0x00){
		  return 1;
	  }else{
		  return 0;
	  }
}

void initialize_mpu(void){

	  //Sets sample rate to 8000/1+7 = 1000Hz
	  write_i2c(MPU6050_RA_SMPLRT_DIV, 0x07);
	  //Disable FSync, 20Hz DLPF
	  write_i2c(MPU6050_RA_CONFIG, 0x04);
	  //Disable gyro self tests, scale of 500 degrees/s
	  write_i2c(MPU6050_RA_GYRO_CONFIG, 0b00001000);
	  //Disable accel self tests, scale of +-2g, no DHPF
	  write_i2c(MPU6050_RA_ACCEL_CONFIG, 0x00);
	  //Motion threshold of 0mg
	  write_i2c(MPU6050_RA_MOT_THR, 0x00);
	  //Disable sensor output to FIFO buffer
	  write_i2c(MPU6050_RA_FIFO_EN, 0x00);

	  //AUX I2C setup
	  //Sets AUX I2C to single master control, plus other config
	  write_i2c(MPU6050_RA_I2C_MST_CTRL, 0x00);
	  //Setup AUX I2C slaves
	  write_i2c(MPU6050_RA_I2C_SLV0_ADDR, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV0_REG, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV0_CTRL, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV1_ADDR, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV1_REG, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV1_CTRL, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV2_ADDR, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV2_REG, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV2_CTRL, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV3_ADDR, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV3_REG, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV3_CTRL, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV4_ADDR, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV4_REG, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV4_DO, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV4_CTRL, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV4_DI, 0x00);

	  //Setup INT pin and AUX I2C pass through.  make int pin open drain.
	  write_i2c(MPU6050_RA_INT_PIN_CFG, 0x40);
	  //disable all interrupts
	  write_i2c(MPU6050_RA_INT_ENABLE, 0x00);

	  //Slave out, dont care
	  write_i2c(MPU6050_RA_I2C_SLV0_DO, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV1_DO, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV2_DO, 0x00);
	  write_i2c(MPU6050_RA_I2C_SLV3_DO, 0x00);
	  //More slave config
	  write_i2c(MPU6050_RA_I2C_MST_DELAY_CTRL, 0x00);
	  //Reset sensor signal paths
	  write_i2c(MPU6050_RA_SIGNAL_PATH_RESET, 0x00);
	  //Motion detection control
	  write_i2c(MPU6050_RA_MOT_DETECT_CTRL, 0x00);
	  //Disables FIFO, AUX I2C, FIFO and I2C reset bits to 0
	  write_i2c(MPU6050_RA_USER_CTRL, 0x00);
	  //Sets clock source to gyro reference w/ PLL
	  write_i2c(MPU6050_RA_PWR_MGMT_1, 0b00000010);
	  //Controls frequency of wakeups in accel low power mode plus the sensor standby modes
	  write_i2c(MPU6050_RA_PWR_MGMT_2, 0x00);

	  //Data transfer to and from the FIFO buffer
	  write_i2c(MPU6050_RA_FIFO_R_W, 0x00);

}



void get_gyro_rates(float* rates){

  int gyro_xout_h = read_i2c(MPU6050_RA_GYRO_XOUT_H);
  int gyro_xout_l = read_i2c(MPU6050_RA_GYRO_XOUT_L);
  int gyro_yout_h = read_i2c(MPU6050_RA_GYRO_YOUT_H);
  int gyro_yout_l = read_i2c(MPU6050_RA_GYRO_YOUT_L);
  int gyro_zout_h = read_i2c(MPU6050_RA_GYRO_ZOUT_H);
  int gyro_zout_l = read_i2c(MPU6050_RA_GYRO_ZOUT_L);

  int gyro_xoffset = 0, gyro_yoffset = 0, gyro_zoffset = 0;

  int gyro_xout = ((gyro_xout_h<<8)|gyro_xout_l) - gyro_xoffset;   //will want to add calibration offsets later.
  int gyro_yout = ((gyro_yout_h<<8)|gyro_yout_l) - gyro_yoffset;
  int gyro_zout = ((gyro_zout_h<<8)|gyro_zout_l) - gyro_zoffset;

  //convert from unsigned to signed
  int gyro_xout_s = (short)gyro_xout;
  int gyro_yout_s = (short)gyro_yout;
  int gyro_zout_s = (short)gyro_zout;

  float gyro_xrate = (float)gyro_xout_s/20.0;  //not sure what these need to be set at.  20?
  float gyro_yrate = (float)gyro_yout_s/20.0;
  float gyro_zrate = (float)gyro_zout_s/20.0;

  float gyro_rates[3] = {gyro_xrate, gyro_yrate, gyro_zrate};

  *rates = gyro_rates[0];
  rates++;
  *rates = gyro_rates[1];
  rates++;
  *rates = gyro_rates[2];

}


void get_accel_values(int* accels){

  int accel_xout_h = read_i2c(MPU6050_RA_ACCEL_XOUT_H);
  int accel_xout_l = read_i2c(MPU6050_RA_ACCEL_XOUT_L);
  int accel_yout_h = read_i2c(MPU6050_RA_ACCEL_YOUT_H);
  int accel_yout_l = read_i2c(MPU6050_RA_ACCEL_YOUT_L);
  int accel_zout_h = read_i2c(MPU6050_RA_ACCEL_ZOUT_H);
  int accel_zout_l = read_i2c(MPU6050_RA_ACCEL_ZOUT_L);

  short accel_xout = (short)((accel_xout_h<<8)|accel_xout_l);
  short accel_yout = (short)((accel_yout_h<<8)|accel_yout_l);
  short accel_zout = (short)((accel_zout_h<<8)|accel_zout_l);

  int accelarr[3] = {accel_xout, accel_yout, accel_zout};
  *accels = accelarr[0];
  accels++;
  *accels = accelarr[1];
  accels++;
  *accels = accelarr[2];

}

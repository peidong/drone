#include <stdio.h>
#include <unistd.h>
#include <mraa.h>

#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C

#define    GYRO_FULL_SCALE_250_DPS    0x00  
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00  
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

int main()
{
	mraa_init();
	mraa_i2c_context mpu;
	mpu = mraa_i2c_init(0);
	mraa_i2c_address(mpu, MPU9250_ADDRESS);
	
	mraa_i2c_write_byte_data(mpu, 0x06, 29);
	mraa_i2c_write_byte_data(mpu, 0x06, 26);
	mraa_i2c_write_byte_data(mpu, ACC_FULL_SCALE_2_G, 28);
	mraa_i2c_write_byte_data(mpu, GYRO_FULL_SCALE_1000_DPS, 27);

	//////////////////////// write_of_slave
	mraa_i2c_write_byte_data(mpu, 32, 106);
	mraa_i2c_write_byte_data(mpu, 0x0C, 49);
	mraa_i2c_write_byte_data(mpu, 10, 50);
	mraa_i2c_write_byte_data(mpu, 22, 51);
	mraa_i2c_write_byte_data(mpu, 128, 52);
	//////////////////////// read_of_slave
	mraa_i2c_write_byte_data(mpu, 32, 106);
	mraa_i2c_write_byte_data(mpu, 140, 37);
	mraa_i2c_write_byte_data(mpu, 3, 38);
	mraa_i2c_write_byte_data(mpu, 136, 39);
	//////////////////////// read_of_slave
	mraa_i2c_write_byte_data(mpu, 18, 10);
	////////////////////////
	
	int sample = 0;
	while(sample<1000)
	{
		uint8_t Buf[14];
		mraa_i2c_read_bytes_data(mpu, 59, Buf, 14);
		// Accelerometer
		int16_t ax = -(Buf[0] << 8 | Buf[1]);
		int16_t ay = -(Buf[2] << 8 | Buf[3]);
		int16_t az = Buf[4] << 8 | Buf[5];
		// Gyroscope
		int16_t gx = -(Buf[8] << 8 | Buf[9]); 
		int16_t gy = -(Buf[10] << 8 | Buf[11]);
		int16_t gz = Buf[12] << 8 | Buf[13];
		// Magnetometer
		mraa_i2c_read_bytes_data(mpu, 73, Buf, 6);
		int16_t mx = -(Buf[1] << 8 | Buf[0])+195;
		int16_t my = -(Buf[3] << 8 | Buf[2])+120;
		int16_t mz = Buf[5] << 8 | Buf[4]-196;
		int result_ag[6] = {ax, ay, az, gx, gy, gz};
		int result_m[3] = {mx, my, mz};
		printf("%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6d,%6d\n",ax,ay,az,gx,gy,gz,mx,my,mz);
   
	}
  return 0;
}
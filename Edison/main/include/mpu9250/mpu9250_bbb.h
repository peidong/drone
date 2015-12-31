#include <stdio.h>
#include <unistd.h>
#include <mraa.h>
#include <math.h>
#include <time.h>


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

#define    CNTL1                     10
#define    ASAX                      16
#define    ASAY                      17
#define    ASAZ                      18
#define    CONFIG                    26
#define    GYRO_CONFIG               27
#define    ACCEL_CONFIG              28
#define    ACCEL_CONFIG_2            29
#define    I2C_SLV0_ADDR             37
#define    I2C_SLV0_REG              38
#define    I2C_SLV0_CTRL             39
#define    I2C_SLV4_ADDR             49
#define    I2C_SLV4_REG              50
#define    I2C_SLV4_DO               51
#define    I2C_SLV4_CTRL             52
#define    I2C_SLV4_DI               53
#define    USER_CTRL                 106

#define    mag_offset_x              195
#define    mag_offset_y              120
#define    mag_offset_z              -196
#define    gyro_offset_x             29
#define    gyro_offset_y             -1
#define    gyro_offset_z             295
#define    acc_offset_x              214
#define    acc_offset_y              -81
#define    acc_offset_z              215

#define PI 3.14159254
mraa_i2c_context mpu;

float mRes = 10.*4912./32760.0;
float aRes = 2.0/32768.0;
float gRes = 500.0/32768.0;

float magCalibration[3] = {0, 0, 0};

float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;   // quaternion of sensor frame relative to auxiliary frame

float beta=3.0f;  // compute beta
float deltat = 0.0025f;   // integration interval for both filter schemes

//---------------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

float invSqrt(float x) {
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}

//====================================================================================================
// MadgwickAHRSupdate
//====================================================================================================

void MadgwickAHRSupdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz) {
    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float hx, hy;
    float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;



    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;   

        // Normalise magnetometer measurement
        recipNorm = invSqrt(mx * mx + my * my + mz * mz);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0mx = 2.0f * q0 * mx;
        _2q0my = 2.0f * q0 * my;
        _2q0mz = 2.0f * q0 * mz;
        _2q1mx = 2.0f * q1 * mx;
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _2q0q2 = 2.0f * q0 * q2;
        _2q2q3 = 2.0f * q2 * q3;
        q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;

        // Reference direction of Earth's magnetic field
        hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
        hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
        _2bx = sqrt(hx * hx + hy * hy);
        _2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
        _4bx = 2.0f * _2bx;
        _4bz = 2.0f * _2bz;

        // Gradient decent algorithm corrective step
        s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    // Integrate rate of change of quaternion to yield quaternion
    q0 += qDot1 * deltat;
  q1 += qDot2 * deltat;
  q2 += qDot3 * deltat;
  q3 += qDot4 * deltat;

    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;
}
//====================================================================================================
// END OF AHRS
//====================================================================================================





void writeAK8963(uint8_t targetReg, uint8_t data)
{
        mraa_i2c_write_byte_data(mpu, 0x0C, I2C_SLV4_ADDR);
    mraa_i2c_write_byte_data(mpu, targetReg, I2C_SLV4_REG);
    mraa_i2c_write_byte_data(mpu, data, I2C_SLV4_DO);
    mraa_i2c_write_byte_data(mpu, 128, I2C_SLV4_CTRL);
}
void readAK8963_setup(uint8_t startReg, int length)
{
      //mraa_i2c_write_byte_data(mpu, 0, I2C_SLV0_CTRL);
        mraa_i2c_write_byte_data(mpu, 0x0C+128, I2C_SLV0_ADDR);
    mraa_i2c_write_byte_data(mpu, startReg, I2C_SLV0_REG);
    mraa_i2c_write_byte_data(mpu, 128+length, I2C_SLV0_CTRL);
}
//////////////////////////////////////////////////////////////////////////
//--->>>>>>> The delay here (usleep) is incredibly important because when
//--->>>>>>> we operate a reg, time need to be spent to write. Otherwise,
//--->>>>>>> the result may come into metastability!!!!!!
//////////////////////////////////////////////////////////////////////////
void initAK8963(float * destination)
{
  // First extract the factory calibration for each magnetometer axis
  uint8_t rawData[3]={0,0,0};  // x/y/z gyro calibration data stored here
  writeAK8963(CNTL1, 0x00);
//  mraa_i2c_write_byte_data(AK8963_ADDRESS, , 0x00, CNTL); // Power down magnetometer
  usleep(10000);
  writeAK8963(CNTL1, 15);
//  mraa_i2c_write_byte_data(AK8963_ADDRESS,, 0x0F, CNTL); // Enter Fuse ROM access mode
//  sleep(0.001);
  usleep(10000);
  readAK8963_setup(16, 3);
  usleep(10000);
  mraa_i2c_read_bytes_data(mpu, 73, rawData, 3);  // Read the x-, y-, and z-axis calibration values
  destination[0] =  (float)(rawData[0] - 128)/256. + 1.;   // Return x-axis sensitivity adjustment values, etc.
  destination[1] =  (float)(rawData[1] - 128)/256. + 1.;
  destination[2] =  (float)(rawData[2] - 128)/256. + 1.;

//  printf("%d,%d,%d\n",rawData[0],rawData[1],rawData[2]);

  writeAK8963(CNTL1, 0x00); // Power down magnetometer
  usleep(10000);
  // Configure the magnetometer for continuous read and highest resolution
  // set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
  // and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
  writeAK8963(CNTL1, 22); // Set magnetometer data resolution and sample ODR
  usleep(10000);
  readAK8963_setup(3, 10); // Setup for the read of Mag from Register 0x03
  usleep(10000);
}

void MPU_init()
{
  // mraa_init();
    mpu = mraa_i2c_init(1);
    // mraa_i2c_frequency(mpu, MRAA_I2C_HIGH);
    mraa_i2c_address(mpu, MPU9250_ADDRESS);
    mraa_i2c_write_byte_data(mpu, 0x01, ACCEL_CONFIG_2);
    mraa_i2c_write_byte_data(mpu, 0x01, CONFIG);
    mraa_i2c_write_byte_data(mpu, ACC_FULL_SCALE_2_G, ACCEL_CONFIG);
    mraa_i2c_write_byte_data(mpu, GYRO_FULL_SCALE_1000_DPS + 0x00, GYRO_CONFIG);
    mraa_i2c_write_byte_data(mpu, 32, USER_CTRL);

  initAK8963(magCalibration);
}
/*
void get_freq()
{
  now = clock();
  deltat = (float)(now-past)/1000000;
  past = now;
//  printf("%f\n",deltat);
}
*/
  

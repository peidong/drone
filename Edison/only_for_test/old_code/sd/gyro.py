import mraa
import time
import math

raw_offset        =         32767
MPU9250_ADDRESS   =         0x68
MAG_ADDRESS       =         0x0C

GYRO_FULL_SCALE_250_DPS  =  0x00  
GYRO_FULL_SCALE_500_DPS  =  0x08
GYRO_FULL_SCALE_1000_DPS =  0x10
GYRO_FULL_SCALE_2000_DPS =  0x18

ACC_FULL_SCALE_2_G     =   0x00  
ACC_FULL_SCALE_4_G     =   0x08
ACC_FULL_SCALE_8_G     =   0x10
ACC_FULL_SCALE_16_G    =   0x18

mpu = mraa.I2c(6)
mag = mraa.I2c(6)

mpu.address(MPU9250_ADDRESS)
mag.address(MAG_ADDRESS)

mpu.writeReg(29,0x06)
mpu.writeReg(26,0x06)
mpu.writeReg(27,GYRO_FULL_SCALE_1000_DPS)
mpu.writeReg(28,ACC_FULL_SCALE_8_G)
time.sleep(1)
buf = [0,0,0,0,0,0,0,0,0,0,0,0,0,0]
gx=[]
gy=[]
gz=[]
ax=[]
ay=[]
az=[]

P = [[0,0],[0,0]]
bias = 0
angle = 0
sample = 0
tmp = 0
res=0

def kalman_filter(newAngle,newRate,dt):
    Q_angle = 0.001 #0.001
    Q_gyroBias = 0.003 #0.003
    R_measure = 0.03 #0.03

    global bias, angle, P
    rate = newRate - bias
    angle += dt * rate
    P[0][0] += dt * (dt*P[1][1] - P[0][1] - P[1][0] + Q_angle)
    P[0][1] -= dt * P[1][1]
    P[1][0] -= dt * P[1][1]
    P[1][1] += Q_gyroBias * dt
    y = newAngle - angle
    S = P[0][0] + R_measure
    K=[0,0]
    K[0] = P[0][0] / S
    K[1] = P[1][0] / S

    angle += K[0] * y
    bias += K[1] * y

    P00_temp = P[0][0];
    P01_temp = P[0][1];

    P[0][0] -= K[0] * P00_temp;
    P[0][1] -= K[0] * P01_temp;
    P[1][0] -= K[1] * P00_temp;
    P[1][1] -= K[1] * P01_temp;
    
    return angle;



while sample<10000:
    for i in range(0,14):
        buf[i]=mpu.readReg(59+i)
        time.sleep(0.001)
    tmp = buf[0]*256 + buf[1] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    ax.append(-tmp+raw_offset)
    tmp = buf[2]*256 + buf[3] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    ay.append(-tmp+raw_offset)
    tmp = buf[4]*256 + buf[5] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    az.append(tmp-raw_offset)
    
    tmp = buf[8]*256 + buf[9] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    gx.append(-tmp+raw_offset);   
    tmp = buf[10]*256 + buf[11] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    gy.append(-tmp+raw_offset);
    tmp = buf[12]*256 + buf[13] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    gz.append(tmp-raw_offset);
    result = [ax[sample],ay[sample],az[sample]]
    
    #print(mraa.getI2cBusId(6))
#    print(result)
#    print(gy)
#    print(gz)
    test = (mpu.readReg(67)*256+mpu.readReg(68))+raw_offset
    if(test > 65536):
        test -= 65536
    test -= raw_offset
    res = kalman_filter(res,test,0.001)
    print (res)
    sample += 1


output = [ax,ay,az]
f=open('demo.txt','w')
for i in output:
    k=' '.join([str(j) for j in i])
    f.write(k+"\n")
f.close()

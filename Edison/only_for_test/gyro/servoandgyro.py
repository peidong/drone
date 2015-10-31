import mraa
import time
import math

x = mraa.Pwm(6)
x.period_us(20000)
x.enable(True)
value= 0

raw_offset        =         30000
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

mpu.writeReg(29,0x08)
mpu.writeReg(26,0x08)
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

sample = 0
tmp = 0
flag = 0;

while sample<10000:
    for i in range(0,14):
        buf[i]=mpu.readReg(59+i)
        #time.sleep(0.01)
    tmp = buf[0]*256 + buf[1] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    gx.append(tmp)
    tmp = buf[2]*256 + buf[3] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    gy.append(tmp)
    tmp = buf[4]*256 + buf[5] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    gz.append(tmp)
    
    tmp = buf[8]*256 + buf[9] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    ax.append(tmp);   
    tmp = buf[10]*256 + buf[11] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    ay.append(tmp);
    tmp = buf[12]*256 + buf[13] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    az.append(tmp);
    result = [gx[sample],gy[sample],gz[sample]]
    
    #print(mraa.getI2cBusId(6))
#    print(result)
#    print(gy)
#    print(gz)

    print(gy[sample])

    value = value + (0.065 + 0.035*(gy[sample]-30000)/4200)
    flag += 1
    if flag == 10:
       value = value/10
       value = round(value,3)
       x.write(value)
       value = 0
       flag = 0
       time.sleep(0.01)
    sample += 1
"""
output = [gx,gy,gz]
f=open('demo.txt','w')
for i in output:
    k=' '.join([str(j) for j in i])
    f.write(k+"\n")
f.close()
"""
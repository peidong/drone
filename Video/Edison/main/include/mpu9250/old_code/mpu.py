import mraa
import time
import math
import numpy

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

mpu = mraa.I2c(0)
mag = mraa.I2c(0)

mpu.address(MPU9250_ADDRESS)
mag.address(MAG_ADDRESS)

mpu.writeReg(29,0x06)
mpu.writeReg(26,0x06)
mpu.writeReg(27,GYRO_FULL_SCALE_1000_DPS)
mpu.writeReg(28,ACC_FULL_SCALE_2_G)

############## write_of_slave
mpu.writeReg(106,32)
mpu.writeReg(49,0x0C)
mpu.writeReg(50,10)
mpu.writeReg(51,22)#mag mode
mpu.writeReg(52,128)
##############


############## read_of_slave
mpu.writeReg(106,32)
mpu.writeReg(37,140)
mpu.writeReg(38,3)
mpu.writeReg(39,136)
##############

mag.writeReg(10,18)



time.sleep(1)
buf = [0,0,0,0,0,0,0,0,0,0,0,0,0,0]
gx=[]
gy=[]
gz=[]
ax=[]
ay=[]
az=[]
mx=[]
my=[]
mz=[]

sample = 0
tmp = 0

def readmag():
    raw_mag = [0,2,0,0,0,0,0]
    if(mag.readReg(2) & 0x01):
        for i in range(0,7):
            raw_mag[i]=mag.readReg(3+i)
    return raw_mag
        
            

while sample<10000:
    for i in range(0,14):
        buf[i]=mpu.readReg(59+i)
        #time.sleep(0.001)
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
    
    
    for i in range(0,6):
        buf[i]=mpu.readReg(73+i)
    tmp = buf[1]*256 + buf[0] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    mx.append(-tmp+raw_offset+195)
    tmp = buf[3]*256 + buf[2] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    my.append(-tmp+raw_offset+120)
    tmp = buf[5]*256 + buf[4] + raw_offset
    if(tmp > 65536):
        tmp -= 65536
    mz.append(-tmp+raw_offset-196)
    
    result = [ax[sample],ay[sample],az[sample],gx[sample],gy[sample],gz[sample]]
    result = [mx[sample],my[sample],mz[sample]]
    test = result[0]*result[0]+result[1]*result[1]+result[2]*result[2]#there is an offset, so the value is not stable
    #print(mraa.getI2cBusId(6))
#    print(result)
#    print(gy)
#    print(gz)
#    test = (mpu.readReg(74)*256+mpu.readReg(73))+raw_offset
#    if(test > 65536):
#        test -= 65536
#    test -= raw_offset
#    test = mag.readReg(2)
#    test = readmag()
#    print(mag.readReg(0))
    print (test)
    sample += 1

output = [mx,my,mz]
f=open('demo.txt','w')
for i in output:
    k=' '.join([str(j) for j in i])
    f.write(k+"\n")
f.close()

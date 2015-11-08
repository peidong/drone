import mraa
import time
import math
import numpy

MPU9250_ADDRESS   =         0x68
MAG_ADDRESS       =         0x0C

mpu = mraa.I2c(0)
mag = mraa.I2c(0)

mpu.address(MPU9250_ADDRESS)
mag.address(MAG_ADDRESS)


############## write_of_slave
mpu.writeReg(106,32)
mpu.writeReg(49,0x0C)
mpu.writeReg(50,10)
mpu.writeReg(51,8)#mag mode
mpu.writeReg(52,128)
##############


############## read_of_slave
mpu.writeReg(106,32)
mpu.writeReg(37,140)
mpu.writeReg(38,0)#which reg to start
mpu.writeReg(39,138)
##############

#mpu.writeReg(0x6B,0)
#mpu.writeReg(55,2)
#mpu.writeReg(55,0)



#mpu.writeReg(0x37,0x02)
#mag.writeReg(0x0A,0x16)
#mpu.readReg(55)
#mpu.writeReg(56,1)
#while True:
#    if(mpu.readReg(73)==1):
#         print("read")
print(mpu.readReg(75))

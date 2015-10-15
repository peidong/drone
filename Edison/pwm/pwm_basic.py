import mraa
import time
x = mraa.Pwm(6)
x.period_us(20000)

x.enable(True)
value= 0.03

while True:

    while value<0.1:
          x.write(value)
          value = value + 0.001
          time.sleep(0.005)
    while value>0.03:
          x.write(value)
          value = value - 0.001
          time.sleep(0.005)

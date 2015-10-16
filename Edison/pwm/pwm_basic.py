import mraa
import time

x = mraa.Pwm(3)
x.period_us(700)
x.enable(True)
value= 0.0

while True:
    x.write(value)
    time.sleep(0.05)
    value = value + 0.01
    if value >= 1:
        value = 0.0

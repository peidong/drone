#LED part
http://fryer.ee.ucla.edu/rest/api/led/?led_status=1&led_on_ms=100&led_off_ms=240

###Database part
#####Database: edison
######table: led
1. id
2. led_status: 0: constant off, 1: constant on, 2: flash
3. led_on_ms: The ms of the led to be on
4. led_off_ms: The ms of the led to be off

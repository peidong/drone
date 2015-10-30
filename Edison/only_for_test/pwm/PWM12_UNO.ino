#include <Wire.h>
void setupPWM16() {
    DDRB |= _BV(PB1) | _BV(PB2);        /* set pins as outputs */
    TCCR1A = _BV(COM1A1) | _BV(COM1B1)  /* non-inverting PWM */
        | _BV(WGM11);                   /* mode 14: fast PWM, TOP=ICR1 */
    TCCR1B = _BV(WGM13) | _BV(WGM12)
        | _BV(CS11);                    /* clk/8 */
    ICR1 = 0x9c40; //40000              /* TOP counter value */
                                        /*16MHz/40000/8 = 50Hz*/
}
void analogWrite16(uint8_t pin, uint16_t val)
{
    switch (pin) {
        case  9: OCR1A = val; break;
        case 10: OCR1B = val; break;
    }
}
void setup() {
    Wire.begin(2);
    Wire.onReceive(receiveEvent); // register event
    Serial.begin(9600);
    setupPWM16();
}
void loop() {
  
}
void receiveEvent(int n)
{
    uint16_t pwm_9 = 0;
    uint16_t pwm_10 = 0;
    uint8_t tmp[4];
    int i=0;
    //Serial.print(Wire.available());
    while(Wire.available()){
      tmp[i]=Wire.read();
      i++;
    }
    pwm_9 = tmp[0]*256+tmp[1];
    pwm_10 = tmp[2]*256+tmp[3];

    analogWrite16(9, pwm_9);
    analogWrite16(10, pwm_10);

}


uint16_t tmp3,tmp4;
int iter=0;
uint16_t pwm_9 = 0;
uint16_t pwm_10 = 0;
char result;

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
void decoder(char r)
{
  int val = r-'0';
  if(val<0)
    iter = 0;
  else{
    switch(iter)
    {
      case 10: tmp3=0;tmp3+=val*10000;break;
      case 11: tmp3+=val*1000;break;
      case 12: tmp3+=val*100;break;
      case 13: tmp3+=val*10;break;
      case 14: tmp3+=val*1;pwm_9=tmp3;break;

      case 15: tmp4=0;tmp4+=val*10000;break;
      case 16: tmp4+=val*1000;break;
      case 17: tmp4+=val*100;break;
      case 18: tmp4+=val*10;break;
      case 19: tmp4+=val*1;pwm_10=tmp4;break;
      default: break;
    }
    iter++;
  }
}

void setup() {
    while(!Serial);
    Serial.begin(115200);
    setupPWM16();
}
void loop() {
    if(Serial.available())
    {
      result = Serial.read();
      decoder(result);
      
      if(pwm_9<=40000&&(iter==14||iter==0)){
        analogWrite16(9, pwm_9);
        //analogWrite16(9, pwm_9);
      }
      if(pwm_10<=40000&&(iter==19||iter==0)){
        analogWrite16(10, pwm_10);
       // analogWrite16(10, pwm_10);
      }
    }

}

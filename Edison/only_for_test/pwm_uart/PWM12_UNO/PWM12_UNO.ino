//#include <Wire.h>
#include <SoftwareSerial.h>
//#include <String.h>

SoftwareSerial mySerial(5, 4); // RX, TX
uint16_t tmp1,tmp2;
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
      case 0: tmp1=0;tmp1+=val*10000;break;
      case 1: tmp1+=val*1000;break;
      case 2: tmp1+=val*100;break;
      case 3: tmp1+=val*10;break;
      case 4: tmp1+=val*1;pwm_9=tmp1;break;

      case 5: tmp2=0;tmp2+=val*10000;break;
      case 6: tmp2+=val*1000;break;
      case 7: tmp2+=val*100;break;
      case 8: tmp2+=val*10;break;
      case 9: tmp2+=val*1;pwm_10=tmp2;break;
      default: break;
    }
    iter++;
  }
}

void setup() {
    while(!Serial);
    Serial.begin(115200);
    mySerial.begin(115200);
    //mySerial.println("Hello, world?");
    setupPWM16();
}
void loop() {
    if(mySerial.available())
    {
      result = mySerial.read();
      decoder(result);
      Serial.write(result);
      
      if(pwm_9<=40000&&(iter==4||iter==0)){
        analogWrite16(9, pwm_9);
      }
      if(pwm_10<=40000&&(iter==9||iter==0)){
        analogWrite16(10, pwm_10);
      }
    }

}

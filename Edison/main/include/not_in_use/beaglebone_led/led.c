#include <mraa.h>
int main(){
    mraa_gpio_context gpio_led;
    gpio_led = mraa_gpio_init_raw(20);
    mraa_gpio_dir(gpio_led, MRAA_GPIO_OUT);
    while(1){
        mraa_gpio_write(gpio_led, 1);
        sleep(1);
        mraa_gpio_write(gpio_led, 0);
        sleep(1);
    }
    mraa_gpio_close(gpio_led);
    return 0;
}
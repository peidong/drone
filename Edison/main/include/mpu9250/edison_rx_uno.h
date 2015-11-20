#include <mraa.h>
#include <stdlib.h>

int16_t acc_x,acc_y,acc_z;
int16_t gyro_x,gyro_y,gyro_z;
int16_t mag_x,mag_y,mag_z;

int myatoi(char c)
{
    if(c>='a'&&c<='f')
        return (c-'a'+10);
    if(c>='0'&&c<='9')
        return (c-'0');
    else
        return 0;
}

int main()
{
    mraa_uart_context uart;
    
    uart = mraa_uart_init(0);
    mraa_uart_set_baudrate(uart, 115200);
    char g[37];
    usleep(1000);
    while(1)
    {
        mraa_uart_read(uart,g,1);
        if(g[0]==' ')
        {
            mraa_uart_read(uart,g,36);
            acc_x = (myatoi(g[0])<<4|myatoi(g[1]))<<8|(myatoi(g[2])<<4|myatoi(g[3]));
            acc_y = (myatoi(g[4])<<4|myatoi(g[5]))<<8|(myatoi(g[6])<<4|myatoi(g[7]));
            acc_z = (myatoi(g[8])<<4|myatoi(g[9]))<<8|(myatoi(g[10])<<4|myatoi(g[11]));

            gyro_x = (myatoi(g[12])<<4|myatoi(g[13]))<<8|(myatoi(g[14])<<4|myatoi(g[15]));
            gyro_y = (myatoi(g[16])<<4|myatoi(g[17]))<<8|(myatoi(g[18])<<4|myatoi(g[19]));
            gyro_z = (myatoi(g[20])<<4|myatoi(g[21]))<<8|(myatoi(g[22])<<4|myatoi(g[23]));

            mag_x = (myatoi(g[26])<<4|myatoi(g[27]))<<8|(myatoi(g[24])<<4|myatoi(g[25]));
            mag_y = (myatoi(g[30])<<4|myatoi(g[31]))<<8|(myatoi(g[28])<<4|myatoi(g[29]));
            mag_z = (myatoi(g[34])<<4|myatoi(g[35]))<<8|(myatoi(g[32])<<4|myatoi(g[33]));
            printf("%d\t,%d\t,%d\t,%d\t,%d\t,%d\t,%d\t,%d\t,%d\t\n",acc_x,acc_y,acc_z,gyro_x,gyro_y,gyro_z,mag_x,mag_y,mag_z);
            // printf("%d\n",myatoi(g[0]));
            // printf("%c\n",g[1]);
            
        }
    }
 
}
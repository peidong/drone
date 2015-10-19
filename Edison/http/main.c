#include "http.h"

int main(void)
{   
    char* content;
    printf("\nStart HTTP GET");
    content = http_get("http://fryer.ee.ucla.edu/rest/api/pwm/get/");
    printf("\n%s", content);
    printf("\n\nStart HTTP POST");
    content = http_post("http://fryer.ee.ucla.edu/rest/api/pwm/post/", "pwm1=1.23&pwm2=2.34&pwm3=3.45&pwm4=4.56");
    printf("\n%s", content);
    return 0;
}

#include "http.h"

int main(void)
{   
    char* content;
    printf("\nStart HTTP GET");
    content = http_get("http://fryer.ee.ucla.edu/rest/api/pwm/get/");
    printf("\n%s", content);
    printf("\n\nStart HTTP POST");
    content = http_post("http://fryer.ee.ucla.edu/rest/api/pwm/post/", "");
    printf("\n%s", content);
    return 0;
}

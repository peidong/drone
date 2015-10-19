#include "http.h"

int main(void)
{       char* content;
       // content = http_get("http://fryer.ee.ucla.edu/rest/api/ip_address/get/");
        content = http_get("http://fryer.ee.ucla.edu/rest/api/pwm/get/");
        printf("/n%s", content);
        
        //http_post("http://fryer.ee.ucla.edu/rest/api/ip_address/post/", "ip_address=192.168.0.1&mac_address=fc:c2:de:37:78:ce&network_name=test");
              return 0;
}

#include "http.h"

int main(void)
{
        http_get("http://fryer.ee.ucla.edu/rest/api/ip_address/get/");
        http_post("http://fryer.ee.ucla.edu/rest/api/ip_address/post/", "ip_address=192.168.0.1&mac_address=fc:c2:de:37:78:ce&network_name=test");
              return 0;
}

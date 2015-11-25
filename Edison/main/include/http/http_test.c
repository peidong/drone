#include "http.h"
#include <unistd.h>/*usleep*/

int main() {
    int i=0;
    while(1) {
        i++;
        printf("begin%d|%send\n", i, http_get("http://fryer.ee.ucla.edu/rest/api/ip_address/get/"));
        usleep(10000);
    }
    return 0;
}

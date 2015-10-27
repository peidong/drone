#include "include/drone_include.h"

int main()
{
    threadpool thpool = thpool_init(10);
    thpool_add_work(thpool, (void*)ThreadTask_HTTP_get_pT_pwm, NULL);
    thpool_add_work(thpool, (void*)ThreadTask_get_arrd_pwm, NULL);
    thpool_wait(thpool);
    thpool_destroy(thpool);
    free_pT_pwm(g_pT_hash_pwm);
    return 0;
}

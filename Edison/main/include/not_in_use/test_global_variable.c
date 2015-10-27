#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct T_test {
    int n1;
    int arrn[2];
    char const *str_2;
};

struct T_test T_test1;

void test(struct T_test *pT_test){
    pT_test->n1 = 100;
    pT_test->arrn[0] = 200;
    pT_test->arrn[1] = 300;
    printf("\n%s", pT_test->str_2);
    pT_test->str_2 = "Chen";
    char const *str_1 = "Peidong ";
    char *str_3 = (char *) malloc(1 + strlen(str_1)+ strlen(pT_test->str_2));
    strcpy(str_3, str_1);
    strcat(str_3, pT_test->str_2);
    printf("\n%s", str_3);
}

int main(){
    /*struct timespec tspec_high;*/
    /*tspec_high.tv_sec = 0;*/
    /*tspec_high.tv_nsec = 500000000L;*/
    T_test1.n1 = 9;
    T_test1.arrn[0] = 19;
    T_test1.arrn[1] = 29;
    T_test1.str_2 = "Haha";
    /*while(1){*/
        /*nanosleep(&tspec_high, NULL);*/
        /*nanosleep(&tspec_high, NULL);*/
        /*printf("1.5s\n");*/
    /*}*/
    printf("n1 = %d, arrn[0] = %d, arrn[1] = %d", T_test1.n1, T_test1.arrn[0], T_test1.arrn[1]);
    test(&T_test1);
    printf("\nn1 = %d, arrn[0] = %d, arrn[1] = %d", T_test1.n1, T_test1.arrn[0], T_test1.arrn[1]);
    return 0;
}

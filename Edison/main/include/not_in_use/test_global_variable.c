#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct T_test {
    int n1;
    int arrn[2];
    char *str_2;
};

struct T_test T_test1;

void test(struct T_test *pT_test){
    pT_test->n1 = 100;
    pT_test->arrn[0] = 200;
    pT_test->arrn[1] = 300;
    pT_test->str_2 = "Chen";
    char *str_1 = "Peidong ";
    char *str_3 = (char *) malloc(1 + strlen(str_1)+ strlen(pT_test->str_2));
    strcpy(str_3, str_1);
    strcat(str_3, pT_test->str_2);
    printf("\n%s", str_3);
}

int main(){
    T_test1.n1 = 9;
    T_test1.arrn[0] = 19;
    T_test1.arrn[1] = 29;
    printf("n1 = %d, arrn[0] = %d, arrn[1] = %d", T_test1.n1, T_test1.arrn[0], T_test1.arrn[1]);
    test(&T_test1);
    printf("\nn1 = %d, arrn[0] = %d, arrn[1] = %d", T_test1.n1, T_test1.arrn[0], T_test1.arrn[1]);
    return 0;
}

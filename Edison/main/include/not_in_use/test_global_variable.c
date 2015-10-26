#include <stdio.h>

struct T_test {
    int n1;
    int arrn[2];
};

struct T_test T_test1;

void test(struct T_test *pT_test){
    pT_test->n1 = 100;
    pT_test->arrn[0] = 200;
    pT_test->arrn[1] = 300;
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

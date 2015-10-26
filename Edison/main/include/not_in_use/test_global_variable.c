#include <stdio.h>

struct T_test {
    int n1;
    int n2;
};

struct T_test T_test1;

void test(struct T_test *pT_test){
    pT_test->n1 = 100;
    pT_test->n2 = 200;
}

int main(){
    T_test1.n1 = 9;
    T_test1.n2 = 19;
    printf("n1 = %d, n2 = %d", T_test1.n1, T_test1.n2);
    test(&T_test1);
    printf("\nn1 = %d, n2 = %d", T_test1.n1, T_test1.n2);
    return 0;
}

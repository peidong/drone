#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */

int main(){
    int n = 1234;
    char arrc_test[8];
    arrc_test[0]='-';
    arrc_test[1]='9';
    arrc_test[2]='3';
    arrc_test[3]='a';
    n = atoi(arrc_test);
    printf("n=%d\n", n);
    /*sprintf(arrc_test, "%d", n);*/
    /*printf("arrc_test[0] = %c\n", arrc_test[0]);*/
    /*printf("arrc_test[1] = %c\n", arrc_test[1]);*/
    /*printf("arrc_test[2] = %c\n", arrc_test[2]);*/
    /*printf("arrc_test[3] = %c\n", arrc_test[3]);*/
	return 0;
}

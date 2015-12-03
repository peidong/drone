#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */

int main(){
    int n = 123;
    char arrc_test[80];
    sprintf(arrc_test, "%d", n);
    printf("%s", arrc_test);
	return 0;
}

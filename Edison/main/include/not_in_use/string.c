#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */

int main(){
    char sz_char[10]="0.012000";
    printf("%f\n", atof(sz_char));
	return 0;
}

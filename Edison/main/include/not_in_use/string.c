#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */

int process_char_float(char *sz_char){
    printf("%f\n", atof(sz_char));
    printf("0: %c\n", sz_char[0]);
    printf("1: %c\n", sz_char[1]);
    printf("2: %c\n", sz_char[2]);
    printf("3: %c\n", sz_char[3]);
    printf("4: %c\n", sz_char[4]);
    printf("5: %c\n", sz_char[5]);
    printf("6: %c\n", sz_char[6]);
    printf("7: %c\n", sz_char[7]);
    printf("8: %c\n", sz_char[8]);
    printf("9: %c\n", sz_char[9]);
    printf("10: %c\n", sz_char[10]);
    printf("11: %c\n", sz_char[11]);
    return 0;
}

int main(){
    char sz_char[10]="0.012000";
    process_char_float(sz_char);
	return 0;
}

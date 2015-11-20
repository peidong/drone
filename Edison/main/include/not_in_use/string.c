#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */

int main(){
	char *char1;
	// char char2[];
	char char3[128];
	char *char4 = "char4";
	char char5[] = "char5";
	char *char6 = (char*)malloc(sizeof("char6"));
	char1 = "char1";
	char1 = "char1_1";
	char4 = "char4_1";
	
	return 0;
}

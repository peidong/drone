#include <stdio.h>
#include <stdlib.h>
#define MAX 100000000
int main(void)
{
    int *a[MAX]={NULL};
    int i;
    for(i=0;i<MAX;i++){
        a[i]=(int*)malloc(MAX);
    }
    return 0;
}

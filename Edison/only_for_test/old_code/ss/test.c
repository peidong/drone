#include <time.h>
#include <stdio.h>

int main()
{
  int now = 0, next = 0, i;
  int t=0;
/*  LOOP:
       next = clock();
       t = next - now;
       now = next;
       for(i = 0; i < 50000000;i++);
       printf("%d\n",t);
  goto LOOP;
*/
  while(1)
  {
       next = clock();
       t = next - now;
       now = next;
       for(i = 0; i < 1000000;i++);
       printf("%d\n",t);
  }

}
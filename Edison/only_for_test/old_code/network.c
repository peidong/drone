#include <stdio.h>
char *hostname;
struct hostent *hostinfo;

hostname = "google.com";
hostinfo = gethostbyname(hostname);

if (hostinfo == NULL) g_print("-> no connection!\n");
else g_print("-> connection established!\n");

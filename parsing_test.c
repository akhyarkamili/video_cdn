#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main () {
   int len = -1;
   char str[80] = "GET index.html HTTP/1.1\r\nHost: 127.0.0.1:8080\r\nContent-Length:800\r\n\r\naaa";
   const char s[5] = "\r\n";
   char *token; char buf[100];
   
   /* get the first token */
   token = strtok(str, s);
   
   /* walk through other tokens */
   while( token != NULL ) {
      if (strstr(token, "Content-Length") != NULL) {
            sscanf(token, "%s %d", buf, &len);
            printf("%s\n", token);
            printf("Length is: %d\n", len);
        }
      token = strtok(NULL, s);
   }

   
   return(0);
}
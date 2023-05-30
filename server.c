#include<stdio.h>
#include <string.h>

typedef struct request{
    char* method;
    char* path;
    char* version;
    char* headers;
    char* body;
} Request;

int main() {                              //
   FILE *fp = fopen("req.txt", "r");
   if (fp == NULL) {
      printf("Error opening file\n");
      return 1;
   }

   //read lines

   char line[1000];

   fgets(line, sizeof(line), fp);
   Request r;

   r.method = strtok(line, " ");
   r.path = strtok(NULL, " ");
   r.version = strtok(NULL, " ");
   printf("method: %s\n", r.method);
   printf("headers %s:", r.method);
   //headers
   char* headers[50];

   int i = 0;
   while (fgets(line, sizeof(line), fp)) {
      //printf("%s", line);
      headers[i] = line;
      i++;
   }

   if(strcmp(r.method, "GET") != 0){
       //remove body from headers
       r.body = headers[i-1];
   }else{
       printf("no body\n");
   }

   return 0;
}

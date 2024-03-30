#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 4096
char *arg_buffer[BUFFER_SIZE];

char **parse_args(char *buffer){
   char *token = NULL;

   token = strtok(buffer, " ");

   uint16_t j = 0;
   while( token != NULL ) {
      arg_buffer[j] = token;
      token = strtok(NULL, " ");
      j++;

      if(j > BUFFER_SIZE){
         return 0;
      }
   }
   /* incase the input is empty */
   if(arg_buffer[0] == NULL){ 
      arg_buffer[0] = "";
   }

   return arg_buffer;
}
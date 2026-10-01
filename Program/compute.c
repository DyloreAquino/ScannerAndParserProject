
/* this program computes sums based on the text file input */

#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include "scan.h"

void parseerror(char *message)
{
   printf("Error: %s\n",message);
   exit(0);
}

int main(int argc, char** argv)
{
   char filename[20];
   struct token t;
   strcpy(filename,"test1.txt");
   if (argc >= 2)
      strcpy(filename,argv[1]);
   openfile(filename);
   t = gettoken();
   while (t.id != TokenEndOfFile)
   {
      int number;
      int sum;
      sum = 0;
      if (t.id != TokenNumber)
         parseerror("Number expected");
      printf("%s\n",t.lexeme);
      sscanf(t.lexeme,"%d",&number);
      sum = number;
      t = gettoken();
      while (t.id == TokenPlus || t.id == TokenMinus) // Add a check for if the token is a minus
      {
         int is_add = t.id == TokenMinus; // This is a boolean to check if we're going to add or not
         t = gettoken();
         if (t.id != TokenNumber)
            parseerror("Number expected");
         printf("%s\n",t.lexeme);
         sscanf(t.lexeme,"%d",&number);
         number = is_add ? -number : number; // Use the is_add to figure out if we're adding or not
         sum += number;
         t = gettoken();
      }
      if (t.id != TokenEqual)
         parseerror("== expected");
      printf("the sum is: %d\n",sum);
      t = gettoken();
   }
   return 0;
}


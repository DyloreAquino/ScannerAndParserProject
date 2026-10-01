
/* this program repeatedly calls gettoken() and prints id and lexeme */

#include <stdio.h>
#include <string.h>
#include "scan.h"

int main(int argc, char** argv)
{
   char filename[50];
   strcpy(filename,"../Tests/InputFiles/sample1-quad-formula.txt");	
   if (argc >= 2)
      strcpy(filename,argv[1]);
   openfile(filename);
   struct token t = gettoken();
   while ( t.id != TokenEndOfFile )
   {
      printf("%s %s\n", tokennames[t.id], t.lexeme);
      t = gettoken();
   }
   return 0;
}


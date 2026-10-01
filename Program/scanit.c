
/* this program repeatedly calls gettoken() and prints id and lexeme */

#include <stdio.h>
#include <string.h>
#include "scan.h"

int main(int argc, char** argv)
{
   char filename[256];
   char outname[300];
   if (argc >= 2)
   {
      for (int i = 1; i < argc; i++)
      {
         strcpy(filename, argv[i]);
         
         char *base = strrchr(filename, '/');
         base = (base != NULL) ? base + 1 : filename;
         strcpy(outname, "Output/");
         strcat(outname, base);
         strcat(outname, "_output_scan.txt");

         openfile(filename);
         freopen(outname, "w", stdout);

         struct token t = gettoken();
         while ( t.id != TokenEndOfFile )
         {
            printf("%s %s\n", tokennames[t.id], t.lexeme);
            t = gettoken();
         }
      }
   }
   else
   {
      strcpy(filename, "sample_input.txt");
      openfile(filename);
      freopen("sample_output_scan.txt", "w", stdout);

      struct token t = gettoken();
      while ( t.id != TokenEndOfFile )
      {
         printf("%s %s\n", tokennames[t.id], t.lexeme);
         t = gettoken();
      }
   }
   return 0;
}


/* this is the main function that calls scan. this is where the files
are being opened and outputted.
*/

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
         
         // This block of code gets the file_name only, excluding the folder path
         // So Inputs/filename will just be filename
         char *base = strrchr(filename, '/');
         base = (base != NULL) ? base + 1 : filename;
         // We then output this to an Output folder
         strcpy(outname, "Output/");
         strcat(outname, base);
         strcat(outname, "_output_scan.txt");

         openfile(filename);
         freopen(outname, "w", stdout); // This line of code allows us to get
         // the printf of this executable into a file
         // https://stackoverflow.com/questions/29154056/redirect-stdout-to-a-file/29154180#29154180 

         struct token t = gettoken();
         while ( t.id != TokenEndOfFile )
         {
            printf("%s %s\n", tokennames[t.id], t.lexeme);
            t = gettoken();
         }
      }
   }
   else // This is an else function for if we call without arguments.
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

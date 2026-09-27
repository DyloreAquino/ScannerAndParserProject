
/* this module defines a gettoken() function      */
/* call openfile("filename") to set up the module */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scan.h"

/* character classes */
// TODO: Add character classes
#define LETTER       0
#define DIGIT        0
#define PLUS         1
#define NEWLINE      2
#define SPACE        3
#define TAB          4
#define EQUAL        5
#define EOFCHAR      6
#define OTHER        7
#define MINUS        8 // Added this for minus 
    
/* state transition table */
// TODO: Add states
int delta[][9] = {
    /*          0,  1,  2,  3,  4,  5,  6,  7,  8   */
    /*  0 */ {  1, 12,  0,  0,  0,  2, 14, 31, 15 }, // Since token id is 5, next state is 15
    /*  1 */ {  1, 21, 21, 21, 21, 21, 21, 21, 21 }, // Same with other pushback
    /*  2 */ { 32, 32, 32, 32, 32, 13, 32, 32, 32 }  // Also same here for error
};

#define MAXLINELEN 1000

FILE *file;
static int linenum = 1;
char line[MAXLINELEN];
int len = 0;
int ptr = 1;
int pushback = FALSE;
char charread = '\0';
    
int openfile(char *filename)
{
    file = fopen(filename,"r");
    if (file == NULL)
    {
       printf("File not found.");
       exit(1);
    }
    return 0;
}

char mygetchar()
{
    if (pushback)
    {
        pushback = FALSE;
    }
    else
    {
    	charread = fgetc(file);
    	if (charread == '\n')
    	   linenum++;
    }
    /*    printf("-->%d\n", charread); */
    return charread;

}
    
int getlinenumber()
{
    return linenum;
}

// TODO: Edit according to character classes
int charclass(char c)
{
    if ((c>='0') && (c<='9'))
        return DIGIT;
    switch(c)
    {
        case  '+': return PLUS;
        case '\r':
        case '\n': return NEWLINE;
        case  ' ': return SPACE;
        case '\t': return TAB;
        case  EOF: return EOFCHAR;
        case  '=': return EQUAL;
        case  '-': return MINUS; // Add this for minus
        default  : return OTHER;
    }
}

// TODO: Edit errors
const char *errormessage(int errnum)
{
    switch(errnum)
    {
        case 31: return "Illegal character";
        case 32: return "Use two equal signs";
        default: return "Unspecified error";
    }
}

struct token gettoken()
{
    int state = 0;
    struct token temp;

    strcpy(temp.lexeme,"");
    char placeholder[] = {'.','\0'}; /* single character string */
    while (state <= 15)
    {
        char c = mygetchar();
        placeholder[0] = c;
        int ch = charclass(c);
        state = delta[state][ch];
        /* printf("class:"+ch+"("+c+") -> state "+ state); */
        if (state == 0)
        /* reset if brought back to state 0 (white spaces and comments) */
            strcpy(temp.lexeme,"");
        else if (state < 20) /* no pushback */
            strcat(temp.lexeme, placeholder);
    }
    if (state > 300) /* greater than 30 means error */
    {
        printf("Lexical Error: %s (line #%d)\n", errormessage(state), linenum);
        temp.id = 0; /* error token */
        strcpy(temp.lexeme,"");
    }
    else if (state >= 200) /* 20 plus means valid token with pushback */
    {
        temp.id = state % 10;
        pushback = TRUE;
    }
    else if (state >= 100) /* 10 plus means valid token with NO pushback */
    {
        temp.id = state % 10;
        pushback = FALSE;
    }
    
    return temp;
}


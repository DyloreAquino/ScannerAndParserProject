
/* this module defines a gettoken() function      */
/* call openfile("filename") to set up the module */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scan.h"

/* character classes */
#define LETTER             0
#define DIGIT              1
#define NEWLINE            2
#define SPACE              3
#define TAB                4
#define EQUAL              5
#define PLUS               6
#define HYPHEN             7
#define ASTERISK           8
#define SLASH              9
#define COLON              10
#define SEMICOLON          11
#define COMMA              12
#define LEFTPAREN          13
#define RIGHTPAREN         14
#define UNDERSCORE         15
#define LESSTHAN           16
#define GREATERTHAN        17
#define EXCLAMATION        18
#define PERIOD             19
#define DOUBLEQUOTE        20
#define EOFCHAR            21
#define OTHER              22
    
/* state transition table */
int delta[16][23] = {
    /*         Lett Digi Newl Spac Tab  Equa Plus Hyph Aste Slas Colo Semi Coma LefP RigP Unds LesT GreT Excl Perd Doub EOFC Other */
    /*   0 */ {   1,   2,   0,   0,   0, 110, 107, 108,  14,  12,  15, 102, 103, 104, 105,   1,  11,  10,   9, 304,   8, 129, 304 },
    /*   1 */ {   1,   1, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214, 214,   1, 214, 214, 214, 214, 214, 214, 214 },
    /*   2 */ { 215,   2, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215,   3, 215, 215, 215 },
    /*   3 */ { 302,   4, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302 },
    /*   4 */ { 215,   4, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215 },
    /*   5 */ { 302,   7, 302, 302, 302, 302,   6,   6, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302 },
    /*   6 */ { 302,   7, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302, 302 },
    /*   7 */ { 215,   7, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215, 215 },
    /*   8 */ {   8,   8, 303,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8, 106, 303,   8 },
    /*   9 */ { 301, 301, 301, 301, 301, 113, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301, 301 },
    /*  10 */ { 220, 220, 220, 220, 220, 112, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220, 220 },
    /*  11 */ { 219, 219, 219, 219, 219, 111, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219 },
    /*  12 */ { 217, 217, 217, 217, 217, 217, 217, 217, 217,  13, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 217 },
    /*  13 */ {  13,  13,   0,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13,  13 },
    /*  14 */ { 216, 216, 216, 216, 216, 216, 216, 216, 109, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216, 216 },
    /*  15 */ { 218, 218, 218, 218, 218, 101, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218, 218 }
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
    linenum = 1;
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

int charclass(char c)
{
    if (((c >= 'a') && (c <= 'z')) || 
        ((c >= 'A') && (c <= 'Z')))
        return LETTER;

    if ((c>='0') && (c<='9'))
        return DIGIT;
    
    switch(c)
    {
        case '\r':
        case '\n': return NEWLINE;
        case  ' ': return SPACE;
        case '\t': return TAB;
        case  '=': return EQUAL;
        case  '+': return PLUS;
        case  '-': return HYPHEN;
        case  '*': return ASTERISK;
        case  '/': return SLASH;
        case  ':': return COLON;
        case  ';': return SEMICOLON;
        case  ',': return COMMA;
        case  '(': return LEFTPAREN;
        case  ')': return RIGHTPAREN;
        case  '_': return UNDERSCORE;
        case  '<': return LESSTHAN;
        case  '>': return GREATERTHAN;
        case  '!': return EXCLAMATION;
        case  '.': return PERIOD;
        case  '"': return DOUBLEQUOTE;
        case  EOF: return EOFCHAR;
        default  : return OTHER;
    }
}

const char *errormessage(int errnum)
{
    switch(errnum)
    {
        case 301: return "'=' expected after '!'";
        case 302: return "Invalid number";
        case 303: return "Unclosed string";
        case 304: return "Illegal character";
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

        if ((c == 'e' || c == 'E') && (state == 2 || state == 4)) {
            state = 5;
        } else {
            state = delta[state][ch];
        }
        
        // printf("class:%d(%c) -> state %d\n", ch, c, state);
        if (state == 0)
        /* reset if brought back to state 0 (white spaces and comments) */
            strcpy(temp.lexeme,"");
        else if (state < 200) /* no pushback */
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
        if (state == 214) {
            if (strcmp(temp.lexeme, "PRINT") == 0)
                temp.id = TokenPRINT;
            else if (strcmp(temp.lexeme, "IF") == 0)
                temp.id = TokenIF;
            else if (strcmp(temp.lexeme, "ELSE") == 0)
                temp.id = TokenELSE;
            else if (strcmp(temp.lexeme, "ENDIF") == 0)
                temp.id = TokenENDIF;
            else if (strcmp(temp.lexeme, "SQRT") == 0)
                temp.id = TokenSQRT;
            else if (strcmp(temp.lexeme, "AND") == 0)
                temp.id = TokenAND;
            else if (strcmp(temp.lexeme, "OR") == 0)
                temp.id = TokenOR;
            else if (strcmp(temp.lexeme, "NOT") == 0)
                temp.id = TokenNOT;
            else   
                temp.id = TokenIdentifier;
        } else {
            temp.id = state % 100;
        }
        
        pushback = TRUE;
    }
    else if (state >= 100) /* 10 plus means valid token with NO pushback */
    {
        temp.id = state % 100;
        pushback = FALSE;
    }
    
    return temp;
}


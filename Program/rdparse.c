// Parser module by Jerold Aquino and Andre Dorde
/* this program is the main paser */

#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "scan.h"

jmp_buf errjmp; // This is important for letting the program continue
// even after getting a parse error, instead of just outright exiting

void parseerror(char *message)
{
	printf("Parse error: %s expected. (line #%i)\n", message, getlinenumber());
	longjmp(errjmp, 1); // Jump back to getting a new file
}

struct token currenttoken;

// Checks if the next token is the token we need
// Prints the token and the lexeme
void match(int tokenid)
{
	if (currenttoken.id != tokenid)
	{
		parseerror(tokennames[tokenid]);
	}
	currenttoken = gettoken();
}

void Prg()
{
	Blk(); match(TokenEndOfFile);
}

void Blk()
{
	if (
		currenttoken.id == TokenIdentifier ||
		currenttoken.id == TokenPRINT ||
		currenttoken.id == TokenIF
	) {
		Stm(); Blk();
	}
}

void Stm()
{
	if (currenttoken.id == TokenIdentifier)
	{
		match(TokenIdentifier);
		match(TokenAssign);
		Exp();
		match(TokenSemicolon);
		printf("Assignment Statement Recognized\n");
	}
	else if (currenttoken.id == TokenPRINT)
	{
		match(TokenPRINT);
		match(TokenLeftParen);
		Arg();
		Argfollow();
		match(TokenRightParen);
		match(TokenSemicolon);
		printf("Print Statement Recognized\n");
	}
	else if (currenttoken.id == TokenIF)
	{
		match(TokenIF);
		printf("If Statement Begins\n");
		Cnd();
		match(TokenColon);
		Blk();
		Iffollow();
		printf("If Statement Ends\n");
	}
	else {
		printf("Invalid Statement\n");
		longjmp(errjmp, 1); // Jump back to getting a new file
	}
}

void Argfollow()
{
	if (currenttoken.id == TokenComma)
	{
		match(TokenComma);
		Arg();
		Argfollow();
	}
}

void Arg()
{
	if (currenttoken.id == TokenString)
	{
		match(TokenString);
	}
	else
	{
		Exp();
	}
}

void Iffollow()
{
	if (currenttoken.id == TokenELSE) {
		match(TokenELSE);
		Blk();
		match(TokenENDIF);
		match(TokenSemicolon);
	} 
	else if (currenttoken.id == TokenENDIF) {
		match(TokenENDIF);
		match(TokenSemicolon);
	}
	else {
		printf("Incomplete if statement\n");
		longjmp(errjmp, 1); // Jump back to getting a new file
	}
}

void Exp()
{
	Trm(); Trmfollow();
}

void Trmfollow()
{
	if (currenttoken.id == TokenPlus || currenttoken.id == TokenMinus)
	{
		if (currenttoken.id == TokenPlus)
		{
			match(TokenPlus);
		}
		else if (currenttoken.id == TokenMinus)
		{
			match(TokenMinus);
		}
		Trm(); Trmfollow();
	}
}

void Trm()
{
	Fac(); Facfollow();
}

void Facfollow()
{
	if (currenttoken.id == TokenMultiply || currenttoken.id == TokenDivide)
	{
		if (currenttoken.id == TokenMultiply)
		{
			match(TokenMultiply);
		}
		else if (currenttoken.id == TokenDivide)
		{
			match(TokenDivide);
		}
		Fac(); Facfollow();
	}
}

void Fac()
{
	Lit(); Litfollow();
}

void Litfollow()
{
	if (currenttoken.id == TokenRaise)
	{
		match(TokenRaise); Lit(); Litfollow();
	}
}

void Lit()
{
	if (currenttoken.id == TokenMinus)
	{
		match(TokenMinus);
	}
	Val();
}

void Val()
{
	if (
		currenttoken.id == TokenIdentifier ||
		currenttoken.id == TokenNumber
	)
	{
		if (currenttoken.id == TokenIdentifier)
		{
			match(TokenIdentifier);
		}
		else
		{
			match(TokenNumber);
		}
	}
	else
	{
		if (currenttoken.id == TokenSQRT)
		{
			match(TokenSQRT);
		}
		match(TokenLeftParen); Exp(); match(TokenRightParen);
	}
}

void Cnd()
{
	Exp(); Rel(); Exp();
}

void Rel()
{
	switch (currenttoken.id)
	{
		case TokenLessThan:
			match(TokenLessThan);
			break;
		
		case TokenEqual:
			match(TokenEqual);
			break;
		
		case TokenGreaterThan:
			match(TokenGreaterThan);
			break;
		
		case TokenGTEqual:
			match(TokenGTEqual);
			break;
		
		case TokenNotEqual:
			match(TokenNotEqual);
			break;
		
		case TokenLTEqual:
			match(TokenLTEqual);
			break;
		
		default:
			printf("Missing relational operator\n");
			longjmp(errjmp, 1); // Jump back to getting a new file
			break;
	}
}

int main(int argc, char** argv)
{
	char filename[256];
	char outname[300];
	if (argc >= 2)
	{
		for (int i = 1; i < argc; i++) {
			strcpy(filename, argv[i]);

			// This block of code gets the file_name only, excluding the folder path
      // So Inputs/filename will just be filename
			char *base = strrchr(filename, '/');
			base = (base != NULL) ? base + 1 : filename;
			// We then output this to an Output folder
			strcpy(outname, "Output/");
			strcat(outname, base);
			strcat(outname, "_output_parse.txt");

			openfile(filename);
			freopen(outname, "w", stdout); // This line of code allows us to get
         // the printf of this executable into a file
         // https://stackoverflow.com/questions/29154056/redirect-stdout-to-a-file/29154180#29154180 

			// this setjmp/longjmp is important for catching parse errors and
			// still allowing the program to continue parsing the rest of the files.
			if (setjmp(errjmp) == 0)
			{
				currenttoken = gettoken();
				Prg();
				printf("%s is a valid SimpCalc program\n", filename);
			}
		}
	}
	else
	{
		strcpy(filename, "sample_input.txt");
		openfile(filename);
		freopen("sample_output_parse.txt", "w", stdout);
		if (setjmp(errjmp) == 0)
		{
			currenttoken = gettoken();
			Prg();
			printf("%s is a valid SimpCalc program\n", filename);
		}
	}
   
	return 0;
}

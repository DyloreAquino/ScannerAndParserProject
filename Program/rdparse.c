
/* this program computes sums based on the text file input */

#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include "scan.h"

char current_state[10] = "none";

void parseerror(char *message)
{
	printf("Parse error: %s (line #%i)\n", message, getlinenumber());
  exit(0);
}

struct token currenttoken;

// Checks if the next token is the token we need
// Prints the token and the lexeme
void match(int tokenid)
{
	if (currenttoken.id != tokenid)
	{
		char msg[100];

		if (strcmp(current_state, "stm") == 0)
			printf("Invalid Statement\n");
		else if (strcmp(current_state, "iffollow") == 0)
			printf("Incomplete if statement\n");
		
		strcpy(msg, tokennames[tokenid]);
		strcat(msg, " expected.");
		parseerror(msg);
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
	)
	{
		Stm(); Blk();
	}
}

void Stm()
{
	strcpy(current_state, "stm");
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
	}
	strcpy(current_state, "none");
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
	strcpy(current_state, "iffollow");
	if (currenttoken.id == TokenELSE)
	{
		match(TokenELSE);
		Blk();
	}

	match(TokenENDIF);
	match(TokenSemicolon);
	strcpy(current_state, "none");
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
			break;
	}
}

int main(int argc, char** argv)
{
   char filename[50];
   strcpy(filename, "../Tests/InputFiles/samp3.txt");
   if (argc >= 2)
      strcpy(filename, argv[1]);
   openfile(filename);
   currenttoken = gettoken();
   Prg();
	 printf("%s is a valid SimpCalc program\n", filename);
   return 0;
}



/* this program computes sums based on the text file input */

#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include "scan.h"

int errorcount = 0;

void parseerror(char *message)
{
	errorcount++;
	printf("Symbol %s expected\n", message);
//    exit(0);
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
	)
	{
		Stm(); Blk();
	}
}

void Stm()
{
	int currenterror = errorcount;

	if (currenttoken.id == TokenIdentifier)
	{
		match(TokenIdentifier);
		match(TokenAssign);
		Exp();
		match(TokenSemicolon);

		if (errorcount == currenterror)
		{
			printf("Assignment Statement Recognized\n");
		}
		else
		{
			printf("Invalid Statement\n");
		}
	}
	else if (currenttoken.id == TokenPRINT)
	{
		match(TokenPRINT);
		match(TokenLeftParen);
		Arg();
		Argfollow();
		match(TokenRightParen);
		match(TokenSemicolon);

		if (errorcount == currenterror)
		{
			print("Print Statement Recognized");
		}
		else
		{
			printf("Invalid Statement\n");
		}
	}
	else if (currenttoken.id == TokenIF)
	{
		match(TokenIF);
		print("If Statement Begins");
		Cnd();
		match(TokenColon);
		Blk();
		Iffollow();
		
		if (errorcount == currenterror)
		{
			print("If Statement Ends\n");
		}
		else
		{
			printf("Invalid Statement\n");
		}
	}
	else
	{
		printf("Invalid Statement\n");
		currenttoken = gettoken();
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
	if (currenttoken.id == TokenELSE)
	{
		match(TokenELSE);
		Blk();
	}

	int currenterror = errorcount;
	match(TokenENDIF);
	match(TokenSemicolon);

	if (errorcount != currenterror)
	{
		printf("Incomplete if Statement\n");
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
			syntaxerror("relational operator"); break;
	}
}

int main(int argc, char** argv)
{
   char filename[20];
   strcpy(filename, "test1.txt");
   if (argc >= 2)
      strcpy(filename, argv[1]);
   openfile(filename);
   currenttoken = gettoken();
   Prg();
   return 0;
}


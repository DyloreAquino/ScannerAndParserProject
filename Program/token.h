
/* structure definition for tokens and #defines for token id constants */
 
struct token
{
	int id;
	char lexeme[1000];
};

extern const char *tokennames[]; /* contains token names for each token below */ 
/* constants representing valid token ids */
#define TokenAssign     	 1
#define TokenSemicolon  	 2
#define TokenComma      	 3
#define TokenLeftParen  	 4
#define TokenRightParen 	 5
#define TokenString			 6
#define TokenPlus       	 7
#define TokenMinus 			 8
#define TokenRaise			 9
#define TokenEqual			10
#define TokenLTEqual     	11
#define TokenGTEqual     	12
#define TokenNotEqual    	13
#define TokenIdentifier  	14
#define TokenNumber      	15
#define TokenMultiply    	16
#define TokenDivide      	17
#define TokenColon       	18
#define TokenLessThan    	19
#define TokenGreaterThan	20
#define TokenPRINT 			21
#define TokenIF  			22
#define TokenELSE        	23
#define TokenENDIF  	 	24
#define TokenSQRT   	 	25
#define TokenAND 	 	 	26
#define TokenOR 	 	 	27
#define TokenNOT	 	 	28
#define TokenEndOfFile   	29

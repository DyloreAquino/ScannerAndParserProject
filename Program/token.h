
/* structure definition for tokens and #defines for token id constants */
 
struct token
{
	int id;
	char lexeme[1000];
};

extern const char *tokennames[]; /* contains token names for each token below */ 
/* constants representing valid token ids */
// TODO: edit dis
#define TokenAssign      1
#define TokenPlus        2
#define TokenEquals      3
#define TokenEndOfFile   4
// I added this as the token id for minus tokens
#define TokenMinus 			 5
#define TokenPRINT 			 21
#define TokenEndOfFile   29

# include <cctype>
# include <iostream>
# include <string>
# include "../includes/tokens.h"
# include "../includes/lexer.h"

using namespace std;

int main()
{
    int token;
    string label;

    while ((token = yylex()) != 0) {
	if (AUTO <= token && token <= WHILE)
	    label = "keyword";

	else if (token == CHARACTER)
	    label = "character";

	else if (token == STRING)
	    label = "string";

	else if (token == ID)
	    label = "identifier";

	else if (token == NUM)
	    label = tolower(yytext[1]) == 'x' ? "hexadecimal" : "decimal";

	else
	    label = "operator";

	cout << label << " " << yytext << endl;
    }
}

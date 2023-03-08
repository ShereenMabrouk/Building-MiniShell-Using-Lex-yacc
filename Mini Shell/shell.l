
/*
 *
 * CS-413 Spring 98
 * shell.l: lexical analyzer for shell
 * You have to extend it.
 *
 */
 /* Shereen Mostafa Hassan Mabrouk   6844
    Hend Khaled Abdelhamid Mohamed Aly 6986
 */

%{

#include <string.h>
#include "y.tab.h"

%}

%%

\n 	{
		return NEWLINE;
	}

[ \t] 	{
		/* Discard spaces and tabs */
     	}	

">" 	{
		return GREAT;
	}
"<" 	{
		return LESS;
	}

"|" {
	return PIPE;
}

">>" {
		return GREATGREAT;
}

"&" {
 	return AMPERSAND;
}
">>&" {
		return GREATGREATAMPERSAND;
}

[^\|\>\<\& \t\n][^\|\>\<\& \t\n]*  {
		/* Assume that file names have only alpha chars */
		yylval.string_val = strdup(yytext);
		return WORD;
	}
	

.	{
		/* Invalid character in input */
		return NOTOKEN;
	}


%%
	

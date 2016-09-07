%{
#include "epsi.h"
#include"parser.tab.h"
extern int linenum;

%}

digit         [0-9]
letter        [a-zA-Z_]

%%
"{"		{ return LBRACKET;}
"}"		{ return RBRACKET;}
":"		{ return COLON;}
";"		{ return SC;}
"."		{ return POINT;}
"+"		{ return PLUS; }
"-"		{ return MINUS; }
"*"		{ return TIMES; }
"/"		{ return DIVISION; }
"("		{ return LPAR; }
")"		{ return RPAR; }

"int"		{ return INT; }
"char" 		{ return CHAR; }
"float"		{ return FLOAT; }
"finput"	{ return FINPUT; }
"foutput"	{ return FOUTPUT; }
"void"		{ return VOID; }
"terminal"	{ return TERMINAL; }



{letter}({letter}|{digit})* {
                       yylval.identifier = strdup(yytext);
                       return IDENTIFIER;      }
{digit}+             { yylval.value = atoi(yytext);
                       return NUM_CONST;     }
[\n]		{linenum++;}
[ \t\r]            /* skip whitespace */

.                    { printf("Unknown character [%c] at line %d.\n",yytext[0],linenum);
                       return 0;    }
%%

int yywrap(void){return 1;}

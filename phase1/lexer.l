%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "../includes/tokens.h"
    #define EVER (;;)
        
%}

digit           [0-9]
alpha           [a-zA-Z]
whitespace      [ \t\n]+  
identifier      ({alpha}|_)({alpha}|{digit}|_)*
decimal         0|[1-9][0-9]*
hex             0[xX](({digit}|[a-fA-F]){1,8})
strg            \"(\\.|[^\\"\n])*\" 
chars           \'(\\.|[^\\'\n])+\' 

%option noyywrap

%%
"/*"            {
                    char c1, c2;
                    for EVER {
                        while ((c1 = yyinput()) != '*' && c1!= EOF)
                            ;   //go through characters in the cmt blk, until '*' found
                        if ((c2 = yyinput()) != '/' && c1!= EOF) {
                            unput(c2);  //char after '*' not '/', in comment 
                        } else  //case cmt blk is ended
                            break;
                    }
                }
{whitespace}    {/*do nothing*/}

"auto"          {return AUTO;}
"break"         {return BREAK;}
"case"          {return CASE;}
"char"          {return CHAR;}
"const"         {return CONST;}
"continue"      {return CONTINUE;}
"default"       {return DEFAULT;}
"do"            {return DO;}
"double"        {return DOUBLE;}
"else"          {return ELSE;}
"enum"          {return ENUM;}
"extern"        {return EXTERN;}
"float"         {return FLOAT;}
"for"           {return FOR;}
"goto"          {return GOTO;}
"if"            {return IF;}
"int"           {return INT;}
"long"          {return LONG;}
"register"      {return REGISTER;}
"return"        {return SHORT;}
"short"         {return SHORT;}
"signed"        {return SIGNED;}
"sizeof"        {return SIZEOF;}
"static"        {return STATIC;}
"struct"        {return STRUCT;}
"switch"        {return SWITCH;}
"typedef"       {return TYPEDEF;}
"union"         {return UNION;}
"unsigned"      {return UNSIGNED;}
"void"          {return VOID;}
"volatile"      {return VOLATILE;}
"while"         {return WHILE;}

{decimal}       {return NUM;}
{hex}           {return NUM;}
{strg}          {return STRING;}
{chars}         {return CHARACTER;}

{identifier}    {return ID;}

"="             {return '=';}
"||"            {return LOR;}
"&&"            {return LAND;}
"|"             {return '|';}
"=="            {return IS_EQ;}
"!="            {return NOT_EQ;}
"<"             {return '<';}
">"             {return '>';}
"<="            {return LTE;}
">="            {return GTE;}
"+"             {return '+';}
"-"             {return '-';}
"*"             {return '*';}
"/"             {return '/';}
"%"             {return '%';}
"&"             {return '&';}
"!"             {return '!';}
"++"            {return INC;}
"--"            {return DEC;}
"->"            {return DEREF_ACC;}
"("             {return '(';}
")"             {return ')';}
"["             {return '[';}
"]"             {return ']';}
"{"             {return '{';}
"}"             {return '}';}
";"             {return ';';}
":"             {return ':';}
"\."            {return '.';}
","             {return ',';}
"\.\.\."        {return TRIPLE_DOT;}

.               {/*do nothing printf("bad character %s\n", yytext);*/}

%%

int
main()
{
    yylex();
    return 0;
}

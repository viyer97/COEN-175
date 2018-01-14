%{
    #include <stdio.h>
    #include <stdlib.h>
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

"auto"          {printf("keyword %s\n", yytext);}
"break"         {printf("keyword %s\n", yytext);}
"case"          {printf("keyword %s\n", yytext);}
"char"          {printf("keyword %s\n", yytext);}
"const"         {printf("keyword %s\n", yytext);}
"continue"      {printf("keyword %s\n", yytext);}
"default"       {printf("keyword %s\n", yytext);}
"do"            {printf("keyword %s\n", yytext);}
"double"        {printf("keyword %s\n", yytext);}
"else"          {printf("keyword %s\n", yytext);}
"enum"          {printf("keyword %s\n", yytext);}
"extern"        {printf("keyword %s\n", yytext);}
"float"         {printf("keyword %s\n", yytext);}
"for"           {printf("keyword %s\n", yytext);}
"goto"          {printf("keyword %s\n", yytext);}
"if"            {printf("keyword %s\n", yytext);}
"int"           {printf("keyword %s\n", yytext);}
"long"          {printf("keyword %s\n", yytext);}
"register"      {printf("keyword %s\n", yytext);}
"return"        {printf("keyword %s\n", yytext);}
"short"         {printf("keyword %s\n", yytext);}
"signed"        {printf("keyword %s\n", yytext);}
"sizeof"        {printf("keyword %s\n", yytext);}
"static"        {printf("keyword %s\n", yytext);}
"struct"        {printf("keyword %s\n", yytext);}
"switch"        {printf("keyword %s\n", yytext);}
"typedef"       {printf("keyword %s\n", yytext);}
"union"         {printf("keyword %s\n", yytext);}
"unsigned"      {printf("keyword %s\n", yytext);}
"void"          {printf("keyword %s\n", yytext);}
"volatile"      {printf("keyword %s\n", yytext);}
"while"         {printf("keyword %s\n", yytext);}

{decimal}       {printf("decimal %s\n", yytext);}
{hex}           {printf("hexadecimal %s\n", yytext);}
{strg}          {printf("string %s\n", yytext);}
{chars}         {printf("character %s\n", yytext);}

{identifier}    {printf("identifier %s\n", yytext);}

"="             {printf("operator %s\n", yytext);}
"||"            {printf("operator %s\n", yytext);}
"&&"            {printf("operator %s\n", yytext);}
"|"             {printf("operator %s\n", yytext);}
"=="            {printf("operator %s\n", yytext);}
"!="            {printf("operator %s\n", yytext);}
"<"             {printf("operator %s\n", yytext);}
">"             {printf("operator %s\n", yytext);}
"<="            {printf("operator %s\n", yytext);}
">="            {printf("operator %s\n", yytext);}
"+"             {printf("operator %s\n", yytext);}
"-"             {printf("operator %s\n", yytext);}
"*"             {printf("operator %s\n", yytext);}
"/"             {printf("operator %s\n", yytext);}
"%"             {printf("operator %s\n", yytext);}
"&"             {printf("operator %s\n", yytext);}
"!"             {printf("operator %s\n", yytext);}
"++"            {printf("operator %s\n", yytext);}
"--"            {printf("operator %s\n", yytext);}
"->"            {printf("operator %s\n", yytext);}
"("             {printf("operator %s\n", yytext);}
")"             {printf("operator %s\n", yytext);}
"["             {printf("operator %s\n", yytext);}
"]"             {printf("operator %s\n", yytext);}
"{"             {printf("operator %s\n", yytext);}
"}"             {printf("operator %s\n", yytext);}
";"             {printf("operator %s\n", yytext);}
":"             {printf("operator %s\n", yytext);}
"\."            {printf("operator %s\n", yytext);}
","             {printf("operator %s\n", yytext);}
"\.\.\."        {printf("operator %s\n", yytext);}

.               {/*do nothing printf("bad character %s\n", yytext);*/}

%%

int
main()
{
    yylex();
    return 0;
}

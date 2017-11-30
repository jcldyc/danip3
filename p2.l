%{
	#include <stdio.h>
	#include "tree.h"
	#include "y.tab.h"
	
	static int lookup (char s[]);
	int line_no = 0;
        int  pos = 1;
	
%}


L	[A-Za-z]
D	[0-9]
%%

[ \t]                   pos += yyleng;  /*Skip white space*/
[\n]                    {line_no++; pos = 1;}



while	 {pos += 5; return While;}
var	 {pos += 3; return Var;}
int 	 {pos += 3; return Int;}
real 	 {pos += 4; return Real;}
boolean  {pos += 7; return Boolean;}
record 	 {pos += 6; return Record;}
end      {pos += 3; return End;}
bind     {pos += 4; return Bind;} 
to	 {pos += 2; return To;}
assert   {pos += 6; return Assert;}
begin    {pos += 5; return Begin;}
loop     {pos += 4; return Loop;}
exit     {pos += 4; return Exit;}
when     {pos += 4; return When;}
if       {pos += 2; return If;}
then     {pos += 4; return Then;}
elsif    {pos += 5; return ElsIf;}
else     {pos += 4; return Else;}
or 	 {pos += 2; return Or;}
and 	 {pos += 3; return And;}
not 	 {pos += 3; return Not;}
not"=" 	 {pos += 4; return NotEqual;}
div 	 {pos += 3; return Div;}
mod 	 {pos += 3; return Mod;}


":" 	 {pos += 1;  return Colon;}
":=" 	 {pos += 2; return Assign;}
"<" 	 {pos += 1; return LessThan;}
">" 	 {pos += 1; return GreaterThan;}
">=" 	 {pos += 2; return GTET;}
"<=" 	 {pos += 2; return LTET;}
"." 	 {pos += 1; return Period;}
"," 	 {pos += 1; return Comma;}

"="     {pos += 1; return Equal; }
"+"     {pos += 1; return Plus; }
"-"     {pos += 1; return Minus; }
"*"     {pos += 1; return Star; }
"/"     {pos += 1; return Slash; }
";"     {pos += 1; return SemiColon; }
"("     {pos += 1; return LPar; }
")"     {pos += 1; return RPar; }


{L}({L}|{D})*            {pos += yyleng; yylval.i = lookup(yytext); return Ident;}
{D}+                     {pos += yyleng; yylval.i = atoi(yytext); return IntConst;}
{D}*\.{D}*               {pos += yyleng; yylval.i = atoi(yytext); return RealConst;}
[+-]?{D}*\.?[Ee][+-]?{D} {pos += yyleng; yylval.i = atoi(yytext); return RealConst;}



. { yyerror ("Unrecognized Character"); }

%%
int yywrap (void) {return 1;}

int  yyerror (char s[])
{
	fprintf (stderr, "Error: %s at token: %s, on line: %d\n", s ,yytext, line_no);
}

typedef char name[15];
static char Names[200][15] = {"<no name>"};
int top = 0;

static int lookup (char s[]){

        int i;

        for(i = 0; i <= top; i++)

                if(strcmp(s, Names[i]) == 0)

                        return i;
                

                strcpy(Names[++top], s);
                return top;
        
}

void printNames (void){

        int i  = 0;

        for(; i <= top; i++){

                printf("%d\t%s\n", i, Names[i]);
        }
}

char *id_name (int i){

        return Names[i];
}
 

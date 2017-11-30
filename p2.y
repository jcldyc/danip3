%{
	#include "tree.h"
	#include <stdlib.h>

	
	extern tree root;
%}

%token <i>	Ident 1 IntConst 2 RealConst 3 
%token	Int 11 Real 12 Boolean 13 Prog 14
%token	End 21 Record 22 Var 23 Bind 24 To 25 Assert 26 Begin 27 Loop 28 Exit 29
%token	When 31 If 32 Then 33 Else 34 ElsIf 35 Or 36 And 37 While 38 
%token	Not 41 NotEqual 42 Equal 43 LessThan 44 LTET 45 GreaterThan 46 GTET 47
%token	Div 51 Mod 52 Plus 53 Minus 54 Star 55 Slash 56
%token	SemiColon 61 Colon 62 Comma 63 Assign 64 LPar 65 RPar 66 Period 67

%start program

%union{ tree p; int i; }
%type <p> pStateDeclSeq idlist type field_list state_decls declaration statement ref end_if expr and_expr not_expr rel_expr sum prod factor basic

%%
program
	: pStateDeclSeq
		{ root = buildTree (Prog, $1, NULL, NULL); }
	;
pStateDeclSeq
	: /*empty*/
		{ $$ = NULL; }
	| statement SemiColon pStateDeclSeq
		{ $$ = buildTree (SemiColon, $1, NULL, NULL); $$->next = $3;}
	| Var idlist Colon type SemiColon pStateDeclSeq
		{$$ = buildTree (Var, $2, $4, NULL); $$->next = $6;}
	;
idlist
	: Ident
		{$$ = buildIntTree(Ident, $1);}
	| Ident Comma idlist
		{$$ = buildIntTree(Ident, $1); $$->next = $3;}
	;
type
	: Int
		{$$ = buildIntTree (Int, 0); }
	| Real
		{$$ = buildIntTree (Real, 0); }
	| Boolean
		{$$ = buildIntTree (Boolean, 0); }
	| Record field_list End Record
		{$$ = buildTree(Record, $2, NULL, NULL);}
	;
field_list
	: idlist Colon type
		{$$ = buildTree(Colon, $1, $3, NULL);}
	| idlist Colon type SemiColon field_list
		{$$ = buildTree(Colon, $1, $3, NULL); $$->next = $5;}
	; 
state_decls
	: /*empty*/
		{$$ = NULL;}
	| statement SemiColon pStateDeclSeq
		{$$ = buildTree(SemiColon, $1, $3, NULL);}
	| declaration SemiColon pStateDeclSeq
		{$$ = buildTree(SemiColon, $1, $3, NULL);}
	;
declaration
	: Var idlist Colon type
		{$$ = buildTree(Var, $2, $4, NULL);}
	| Bind Ident To ref
		{$$ = buildTree(Bind, buildIntTree(Ident, $2), $4, NULL);}
	| Bind Var Ident To ref
		{$$ = buildTree(Bind, buildIntTree(Ident, $3), $5,NULL);}
	;
statement
	: ref Assign expr
		{$$ = buildTree(Assign, $1, $3, NULL);}
	| Assert  expr
		{$$ = buildTree(Assert, $2, NULL, NULL);}
	| Begin state_decls End
		{$$ = buildTree(Begin, $2, NULL, NULL);}
	| Loop state_decls End Loop
		{$$ = buildTree(Loop, $2, NULL, NULL);}
	| Exit
		{}
	| Exit When expr
		{$$ = buildTree(Exit, NULL, NULL, NULL);}
	| If expr Then state_decls end_if
		{$$ = buildTree(If, $2, $4, NULL);}
	;
ref
	: Ident
		{$$ = buildIntTree(Ident, $1);}
	| Ident Period Ident
		{$$ = buildTree(Period, buildIntTree(Ident, $1), buildIntTree(Ident, $3), NULL);}
	;
end_if
	:End If 
		{}
	| Else state_decls end_if
		{$$ = buildTree(Else, $2, NULL, NULL);}
	| ElsIf  expr Then state_decls end_if
		{$$ = buildTree(ElsIf, $2, $4, NULL); $$->next = $5;}
	;
expr
	: expr Or and_expr
		{$$ = buildTree(Or, $1, $3, NULL);}
	| and_expr
		{$$ = $1;}
	;
and_expr
	: and_expr And not_expr
		{$$ = buildTree(And, $1, $3, NULL);}
	| not_expr
		{$$ = $1;}
	;
not_expr
	: Not not_expr
		{$$ = buildTree(Not, $2, NULL, NULL);}
	| rel_expr
		{$$ = $1;}
	;
rel_expr
	: sum
		{$$ = $1;}
	| rel_expr Equal sum
		{$$ = buildTree(Equal, $1, $3, NULL);}
	| rel_expr NotEqual sum
		{$$ =  buildTree(NotEqual, $1, $3, NULL);}
	| rel_expr LessThan sum
		{$$ = buildTree(LessThan, $1, $3, NULL);}
	| rel_expr LTET sum
		{$$ = buildTree(LTET, $1, $3, NULL);}
	| rel_expr GreaterThan sum
		{$$ = buildTree (GreaterThan, $1, $3, NULL);}
	| rel_expr GTET sum
		{$$ = buildTree(GTET, $1, $3, NULL);}
	;
sum
	: prod
		{$$ = $1;}
	| sum Plus prod
		{$$ = buildTree(Plus, $1, $3, NULL);}
	| sum Minus prod
		{$$ = buildTree(Minus, $1, $3, NULL);}
	;
prod
	: factor
		{$$ = $1;}
	| prod Star factor
		{$$ = buildTree(Star, $1, $3, NULL);}
	| prod Slash factor
		{$$ = buildTree(Slash, $1, $3, NULL);}
	| prod Div factor
		{$$ = buildTree(Div, $1, $3, NULL);}
	| prod Mod factor
		{$$ = buildTree(Mod, $1, $3, NULL);}
	;
factor
	: Plus basic
		{$$ = buildTree(Plus, $2, NULL, NULL);}
	| Minus basic
		{$$ = buildTree(Minus, $2, NULL, NULL);}
	| basic
		{$$ = $1;}
	;
basic
	: ref
		{$$ = $1;}
	| LPar expr RPar
		{$$ = $2;}
	| IntConst
		{$$ = buildIntTree(IntConst, $1);}
	| RealConst
		{$$ = buildRealTree(RealConst, $1);}
	;

%%

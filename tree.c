#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "y.tab.h"

tree buildTree (int kind, tree one, tree two, tree three)
{
	tree p = (tree)malloc(sizeof (node));
	p->kind = kind;
	p->one = one;
	p->two = two;
	p->three = three;
	p->next = NULL;
	return p;
}

tree buildIntTree (int kind, int val)
{
	tree p = (tree)malloc(sizeof (node));
	p->kind = kind;
	p->value = val;
	p->one = p->two = p->three = NULL;
	p->next = NULL;
	return p;
}
tree buildRealTree (int kind, float num)
{
	tree p = (tree)malloc(sizeof(node));
	p->kind = kind;
	p->f = num;
	p->one = p->two = p->three = NULL;
	p->next = NULL;
	
	return p;
}

char TokName[][12] =
                {"<eof>",
                "Ident", "IntConst", "RealConst", "", "", "", "", "", "", "",
                "<IntType>", "<RealType>","<BoolType>", "<Prog>", "", "", "", "", "", "",
                "End", "Record", "Var", "Bind", "To", "Assert", "Begin", "Loop", "Exit", "",
                "When", "If", "Then", "Else", "ElsIf", "Or", "And", "", "", "",
                "Not", "NotEqual", "=", "<", "<=", ">", ">=", "", "", "",
                "Div", "Mod", "+", "-", "*", "/", "", "", "", "",
                ";", ":", ",", ":=", "(", ")", ".", "", "", ""};

                

static int indent = 0;
void printTree (tree p)
{
	if (p == NULL) return;
	for (; p != NULL; p = p->next) {
		printf ("%*s%s", indent, "", TokName[p->kind]);
		switch (p->kind) {
			case Ident: 
				printf ("  %s (%d)\n", id_name (p->value), p->value);
				break;
			case IntConst:
				printf ("  %d\n", p->value);
				break;
			case RealConst:
				printf (" %s\n", p->value);
				break;
			default:
				printf ("\n");
				indent += 2;
				printTree (p->one);
				printTree (p->two);
				printTree (p->three);
				indent -= 2;
			}
		}
}

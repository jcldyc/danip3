#include <stdio.h>
#include "tree.h"
#include "y.tab.h"
#include "st.h"

extern int top;
STentry ST[100] = {0, 0, 0, 0};
int scope = 0;

void prST (void){
	
	int i;
	printf("SYMBOL TABLE\n");
	
	for(i = 0; i <= top; i++){
		int t = ST[i].type;
		printf("%3d %-10s\t%s\t\%d\n", i, id_name(i),
			t == Int ? "int" : t == Boolean ? "boolean" : t == Real ? "real" : "<none>", ST[i].scope);
	}
}

static int check_expr (tree t){

	if (t == NULL){
	
		fprintf(stderr, "You shouldn't be here; missing expression\n");
		return 0;
	}

	switch(t->kind){

			case NotEqual: case Equal:
			case LessThan: case LTET:
			case GreaterThan: case GTET:
			{
				int t1 = check_expr(t->one);
				int t2 = check_expr(t->two);

				if(t1 == t2){
				
					return (t->value = Boolean);
				}
				else if(t1 == Real || t2 == Int){
				
					return(t->value = Real);
				}
				else if(t1 == Int || t2 == Real){
				
					return(t->value = Real);
				}
				else{
					
					fprintf(stderr, "Type mismatch in comparison\n");
					return (t->value = 0);
				}
			}
			break;
			
			case Plus: case Minus:
			case Star: case Mod:
			{

				int t1 = check_expr(t->one);
				int t2 = check_expr(t->two);

				if( t1 == Real && t2 == Real){
				
					return(t->value = Real);
				}
				else if (t1 == Int && t2 == Int){
				
					return(t->value = Int);
				}
				else{
				
					fprintf(stderr, "Type mismatch in binary arithmetic expression\n");
					return(t->value = 0);
				}
			}
			break;

			case Slash:
			{

				int t1 = check_expr(t->one);
				int t2 = check_expr(t->two);

                                if( t1 != t2){                        

                                        fprintf(stderr, "Type mismatch in arithmetic expression\n");
                                        return(t->value = 0);
                                }
				else{
				
					return(t->value = Real);
				}
			}
			break;

			case Div:
			{

				int t1 = check_expr(t->one);
				int t2 = check_expr(t->two);

                                if( t1 != t2){                        

                                        fprintf(stderr, "Type mismatch in arithmentic expression\n");
                                        return(t->value = 0);
                                }
				else{
				
					return(t->value = Int);
				}
			}
			break;

			case And: case Or:
			{

				int t1 = check_expr(t->one);
				int t2 = check_expr(t->two);

				if(t1 != Boolean || t2 != Boolean){
				
					fprintf(stderr, "Type mismatch in And expression\n");
					return(t->value = 0);
				}
				else{
				
					return(t->value = Boolean);
				}
			}

			case Not:
			{

				int t1 = check_expr(t->one);
			
				if(t1 == Boolean){
				
					return(t->value = Boolean);
				}
				else{
				
					fprintf(stderr, "Type mismatch in expression\n");
					return(t->value = 0);
				}
			}

			case IntConst:
			{

				return Int;
			}

			case RealConst:
			{

				return Real;
			}

			case Ident:
			{

				int pos  = t->value;
				if(ST[pos].index == 0){
				
					ST[pos].index = pos;
					ST[pos].type = Int;
					ST[pos].scope = scope;
				}
				return ST[pos].type;
				break;
			}
			
			default:
			{

				fprintf(stderr, "You shouldn't be here; invalid expression operator\n");
			}
	
	}
}

static void handle_decls (tree t){

	for(t; t != NULL; t = t->next){
	
		int type = t->kind;
		tree p;
		if(type != Int && type != Boolean && type != Real){
		
			fprintf(stderr, "Bad type in decl\n");
			return;
		}
		for (p = t->one; p != NULL; p = p->next){
		
			int pos = p->value;
			ST[pos].index = pos;
			ST[pos].type = type;
			ST[pos].scope = scope;
		}
	}
}

void check (tree t){

	for(t; t != NULL; t = t->next){
	
		switch (t->kind){
		
			case Prog:
			{
				handle_decls (t->one);
				check (t->two);
				printf ("\n");
				prST();
				break;
			}
			
			case SemiColon:
			{
				check(t->one);
				if(t->two != NULL){
				
					check(t->two);
				}
				break;
			}

			/*case Var:
 * 			{
 * 							
 * 										}*/

			case Int:
			{

				if(check_expr(t->one) != Int){
				
					fprintf(stderr, "Type error in Type\n");
				}
				break;
			}

			case Real:
			{

				if(check_expr(t->one) != Real){
				
					fprintf(stderr, "Type error in Type\n");
				}
				break;
			}

			case Boolean:
			{

				if(check_expr(t->one) != Boolean){
				
					fprintf(stderr, "Type error in Type\n");
				}
				break;
			}

			/*case Record:
 * 			{
 * 						}
 *
 * 									case Colon:
 * 												{
 *
 * 																
 * 																			}
 *
 * 																						case Bind:
 * 																									{
 * 																												}*/

			case Assign:
			{

				 int pos = t->one->value;

                                if(ST[pos].index == 0){

                                        ST[pos].index = pos;
                                        ST[pos].type = Int;
                                        ST[pos].scope = scope;
                                }
                                if(check_expr(t->one) != ST[pos].type){

                                        fprintf(stderr, "Type error in assignment condition\n");
                                }
			}

			case If:
			{
				scope++;
				if(check_expr(t->one) != Boolean){
				
					fprintf(stderr, "Non-boolean in condition to If\n");
				}
				check(t->two);
                               	check(t->three);
				scope --;
                                break;
			}
			
			case Loop:
			{
				scope++;
				if(check_expr(t->one) != Boolean){
				
					fprintf(stderr, "Non-boolean in condition to Loop\n");
				}
				check(t->two);
				scope--;
				break;
			}

			case Begin:
			{
				scope++;
				if(check_expr(t->one) != Boolean){
				
					fprintf(stderr, "Non-boolean in condition to Begin\n");
				}
				check(t->two);
				scope--;
				break;
			}

			case ElsIf:
			{

				scope++;
				if(check_expr(t->one) != Boolean){
				
					fprintf(stderr, "Non-boolean in condition for ElsIf\n");
				}
				check(t->two);
				check(t->three);
				scope--;
				break;
			}

			case Else:
			{

				scope++;
				handle_decls(t->one);
				check(t->two);
				scope--;
				break;
			}

			case Assert:
			{

				check(t->one);
				if(check_expr(t->two) != Boolean){
				
					fprintf(stderr, "Non-boolean in comnition for Assert\n");
				}
				break;
			}

			case Exit:
			{

				check(t->one);
				scope--;
				break;
			}

			case When:
			{

				if(check_expr(t->one) != Boolean){
				
					fprintf(stderr, "Non_boolean in condition for When\n");
				}
				break;
			}

			case Period:
			{

				check(t->one);
				if(check_expr(t->two) != Int || check_expr(t->three) != Int){
				
					fprintf(stderr, "Type mismatch in condition FullStop\n");
				}
				break;
			}

			default:
			{

				fprintf(stderr, "You shouldn't be here; invalid statement\n");
			}
		}
	}
}


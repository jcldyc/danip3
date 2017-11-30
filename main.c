#include <stdio.h>
#include <stdlib.h>
#include "tree.h"



extern FILE *yyin;

tree root;

FILE	*outfile;

main (int argc, char **argv)
{

	if (argc != 2) 
	{ 
		fprintf (stderr, "%s: Insufficient Arguments\n", argv[0]); 
		exit(1);
	}
	if ((yyin = fopen (argv[1], "r")) == 0L) 
	{
		fprintf (stderr, "%s: Can't open Input File %s\n", argv[0], argv[1]); 
		exit(1);
	}

	yyparse();
// printNames ();
// printTree(root);
//buildSymT(root);
check(root);
	
	
 
          /*   if ((outfile = fopen (argv[2], "w")) == 0L) {
              fprintf (stderr, "%s: Can't open Outfile File %s\n", argv[0], argv[2]); 
              exit(1);
              }*/

        
	close (outfile);
	close (yyin);
}

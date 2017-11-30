typedef struct Node {
	int	kind, value;
	float f;
	struct	Node *one, *two, *three, *next;
} node;
typedef node	*tree;

extern char TokName[][12];



tree buildTree (int kind, tree one, tree two, tree three);
tree buildIntTree (int kind, int val);
tree buildRealTree(int kind, float num);
void printTree (tree);

/*
 * void check (tree);
 *
 * void gen_program (tree);
 *
  */


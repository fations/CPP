#include "project3.h"
#include "true_false.h"

int xpos,ypos;
square * loc[10][10];
logical_expression * knowledge_base;

game * creategame() {

	game * g = new game;
	square * n;
	int i,j;
	
	for (i=0;i<10;i++) {
		for (j=0;j<10;j++) {
			n = new square;
			n->xpos = i+1;
			n->ypos = j+1;
			loc[i][j] = n;
			
			if ((i==0 && j==9) || (i==0 && j==8) || (i==1 && j==9) || (i==1 && j==8) || (i==8 && j==8) || (i==7 && j==2))
				n->g = 1;
			else
				n->g = 0;
		}
	}

	return  g;
}


int move (square * start, int dir) {

	int x,y;

	x = n->xpos;
	y = n->ypos;

	if (dir == 1) {

		if (y<10) {
			xpos = x;
			ypos = y + 1;
		}
		else
			return 0;
	}
	else if (dir == 2) {

		if (x<10) {
			xpos = x + 1;
			ypos = y;
		}
		else
			return 0;
	}
	else if (dir == 3) {

		if (y>1) {
			xpos = x;
			ypos = y - 1;
		}
		else
			return 0;
	}
	else if (dir == 4) {

		if (x>1) {
			xpos = x - 1;
			ypos = y;
		}
		else
			return 0;
	}

	return 1;
}

int pcount(square *s) {
	int i = 0;
	square * tmp;

	tmp = s;

	while (tmp->parent) {
		i++;
		tmp = tmp->parent;
	}

	return i;
}

logical_expression * formstatement(square *n) {
	
	logical_expression * statement;
	char buffer[200];

	//read knowledge base
	input_file = fopen("statement.txt", "rb");
	if (input_file == NULL) {
		printf("failed to open statement file\n");
		return exit_function(0);
	}

	printf("\n\nLoading statement...\n");
	fgets(buffer, 200, input_file);
	fclose(input_file);

	statement = read_expression(buffer);
	if (valid_expression(statement) == 0) {
		printf("invalid statement\n");
		return exit_function(0);
	}

	print_expression(statement, "");

	check_true_false(knowledge_base, statement);

	return statement;
}

int playgame(game * g) {

	square * n;
	int i;
	bool gflag, mflag;

	gflag = mflag = 0;

	while (!gflag) {
		
		n = loc[xpos-1][ypos-1];
		i = 0;

		while (i<4 && !mflag) {
			if (check(n,i+1) == 1)  {
				move(n,i+1);
				mflag = 1;
				
				if (n->g == 1)
					gflag = 1;
			}

			i++;
		}
	}	
}

int main() {
	
	game * g, *tmp;
	char buffer[200];
	FILE * input_file;
	
	g = creategame();

	//read wumpus rules
	input_file = fopen("rules.txt", "rb");
	
	if (input_file == NULL) {
		printf("failed to open rules file\n");
		return exit_function(0);
	}

	printf("Loading wumpus rules...\n");
	knowledge_base = new logical_expression();
	strcpy(knowledge_base->connective, "and");
	
	while(fgets(buffer, 200, input_file) != NULL) {
		logical_expression * subexpression = read_expression(buffer);
		knowledge_base->subexpressions.push_back(subexpression);
	}
	fclose(input_file);
  
	// read additional knowledge

	input_file = fopen("kb.txt", "rb");
	if (input_file == NULL) {
		printf("failed to open kb file\n");
		return exit_function(0);
    }

	printf("Loading additional knowledge...\n");
	while(fgets(buffer, 200, input_file) != NULL) {
		logical_expression * subexpression = read_expression(buffer);
		knowledge_base->subexpressions.push_back(subexpression);
	}
	fclose(input_file);
  
	if (valid_expression(knowledge_base) == 0) {
		printf("invalid knowledge base\n");
		return exit_function(0);
	}

	print_expression(knowledge_base, "\n");

	playgame(g);

	printf("\n\nGame over...\n");
	getchar();

	tmp = g;

	while (tmp->child) {
		updatelocator(tmp);
		printf("\n\nStep...\n");
		printgame();
		getchar();
		tmp = tmp->child;	
	}

	printf("\n\nFinish...\n");
	getchar();

	delete knowledge_base;
	delete statement;
	exit_function(1);
}
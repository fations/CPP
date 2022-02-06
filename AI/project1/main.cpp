
#include "project1.h"
using namespace std;

int newmaze[25][25];
queue<Node*> q,qo,qc,qa;
bool flag = 0;
bool flag2 = 0;
Node * fin;

int checkq(Node * n, int x, int y) {

	int qsize = q.size();
	int i;
	Node * m;

	for (i=0;i<qsize;i++) {
		m = new Node;
		m->blocked = q.front()->blocked;
		m->top = q.front()->top;
		m->right = q.front()->right;
		m->bottom = q.front()->bottom;
		m->left = q.front()->left;
		m->parent = q.front()->parent;
		m->xpos = q.front()->xpos;
		m->ypos = q.front()->ypos;

		if (m->xpos == x && m->ypos == y) {
			//printf("\nAlready in q, not adding X:%d , Y:%d",m->xpos,m->ypos);
			m->parent = n;
			//getchar();
			return 1;
		}

		q.pop();
		q.push(m);
	}

	return 0;
}

int checkqc(Node * n, int x, int y) {

	int qsize = qc.size();
	int i;
	Node * m;

	for (i=0;i<qsize;i++) {
		m = new Node;
		m->blocked = qc.front()->blocked;
		//m->top = qc.front()->top;
		//m->right = qc.front()->right;
		//m->bottom = qc.front()->bottom;
		//m->left = qc.front()->left;
		m->parent = qc.front()->parent;
		m->xpos = qc.front()->xpos;
		m->ypos = qc.front()->ypos;

		if (m->xpos == x && m->ypos == y) {
			//printf("\nAlready in closed q, not adding X:%d , Y:%d",m->xpos,m->ypos);
			m->parent = n;
			//getchar();
			return 1;
		}

		qc.pop();
		qc.push(m);
	}

	return 0;
}

int checkqo(Node * n, int x, int y) {

	int qsize = qo.size();
	int i;
	Node * m;

	for (i=0;i<qsize;i++) {
		m = new Node;
		m->blocked = qo.front()->blocked;
		//m->top = qo.front()->top;
		//m->right = qo.front()->right;
		//m->bottom = qo.front()->bottom;
		//m->left = qo.front()->left;
		m->parent = qo.front()->parent;
		m->xpos = qo.front()->xpos;
		m->ypos = qo.front()->ypos;

		if (m->xpos == x && m->ypos == y) {
			//printf("\nAlready in open q, not adding X:%d , Y:%d",m->xpos,m->ypos);
			m->parent = n;
			//getchar();
			return 1;
		}

		qo.pop();
		qo.push(m);
	}

	return 0;
}

Node * addtoq(Node * n, int x, int y) {

	Node * m;

	m = new Node;
	m->blocked = newmaze[y][x];

	if (m->blocked == 1)
		return NULL;

	m->xpos = x;
	m->ypos = y;
	m->parent = n;
	
	q.push(m);

	//printf("\nAdded to the queue X:%d , Y:%d",x,y);
	//getchar();

	return m;
}

Node * addtoqo(Node * n, int x, int y) {

	Node * m;

	m = new Node;
	m->blocked = newmaze[y][x];

	if (m->blocked == 1)
		return NULL;

	m->xpos = x;
	m->ypos = y;
	m->parent = n;
	
	qo.push(m);

	//printf("\nAdded to the queue X:%d , Y:%d",x,y);
	//getchar();

	return m;
}

Node * addtoqa(Node * n, int x, int y) {

	Node * m;

	m = new Node;
	m->blocked = newmaze[y][x];

	if (m->blocked == 1)
		return NULL;

	m->xpos = x;
	m->ypos = y;
	m->parent = n;
	
	qa.push(m);

	//printf("\nAdded to the queue X:%d , Y:%d",x,y);
	//getchar();

	return m;
}

int DFS (Node *n, int x, int y) {
	
	Node *top, *right, *bottom, *left;
	int c;
	
	if (flag == 1)
		return 1;

	if (n->xpos == x && n->ypos == y) {
		flag = 1;
		printf("\nFOUND\n");
		fin = n;
		return 1;
	}
	else {
		if (n->ypos<24) {
			c = checkq(n,n->xpos,n->ypos+1);
			if (c == 0)
				top = addtoq(n,n->xpos,n->ypos+1);
		
			if (top != NULL && c == 0) DFS(top,x,y);
		}
		else top = NULL;
		if (n->xpos<24) {
			c = checkq(n,n->xpos+1,n->ypos);
			if (c == 0)
				right = addtoq(n,n->xpos+1,n->ypos);
		
			if (right != NULL && c == 0) DFS(right,x,y);
		}
		else right = NULL;
		if (n->ypos>0) {
			c = checkq(n,n->xpos,n->ypos-1);
			if (c == 0)
				bottom = addtoq(n,n->xpos,n->ypos-1);

			if (bottom != NULL && c == 0) DFS(bottom,x,y);
		}
		else bottom = NULL;
		if (n->xpos>0) {
			c = checkq(n,n->xpos-1,n->ypos);
			if (c == 0)
				left = addtoq(n,n->xpos-1,n->ypos);

			if (left != NULL && c == 0) DFS(left,x,y);
		}
		else left = NULL;
	}

	return 0;
}

int BFS(int x, int y) {

	Node *m;
	int c1, c2;

	while (qo.size()>0) {
		
		m = qo.front();

		if (m->ypos<24) {
			c1 = checkqc(m,m->xpos,m->ypos+1);
			c2 = checkqo(m,m->xpos,m->ypos+1);
			
			if (c1 == 0 && c2 == 0)
				addtoqo(m,m->xpos,m->ypos+1);
		}
		if (m->xpos<24) {
			c1 = checkqc(m,m->xpos+1,m->ypos);
			c2 = checkqo(m,m->xpos+1,m->ypos);
			
			if (c1 == 0 && c2 == 0)
				addtoqo(m,m->xpos+1,m->ypos);
		}
		if (m->ypos>0) {
			c1 = checkqc(m,m->xpos,m->ypos-1);
			c2 = checkqo(m,m->xpos,m->ypos-1);
			
			if (c1 == 0 && c2 == 0)
				addtoqo(m,m->xpos,m->ypos-1);
		}
		if (m->xpos>0) {
			c1 = checkqc(m,m->xpos-1,m->ypos);
			c2 = checkqo(m,m->xpos-1,m->ypos);
			
			if (c1 == 0 && c2 == 0)
				addtoqo(m,m->xpos-1,m->ypos);
		}

		m = new Node;
		m->blocked = qo.front()->blocked;
		//m->top = qo.front()->top;
		//m->right = qo.front()->right;
		//m->bottom = qo.front()->bottom;
		//m->left = qo.front()->left;
		m->parent = qo.front()->parent;
		m->xpos = qo.front()->xpos;
		m->ypos = qo.front()->ypos;

		if (m->xpos == x && m->ypos == y) {
			printf("\nFOUND\n");
			fin = m;
			return 1;
		}

		qo.pop();
		qc.push(m);
	}

	return 0;
}

int manD(Node *n, int goalx, int goaly) {

	return (abs(n->xpos - goalx) + abs(n->ypos - goaly));
}

int euclidD(Node *n, int goalx, int goaly) {

	return (sqrt((n->xpos - goalx)*(n->xpos - goalx) + (n->ypos - goaly)*(n->ypos - goaly)));
}

int pathCost(Node *n) {
	int pc = 0;
	Node *q;

	q = n;

	while (q->parent != NULL) {
		q = q->parent;
		pc++;
	}

	return pc;
}

Node * searchqastar() {

	Node *curr, *best, *pop;
	int qsize, i;
	int bestd = 1000;

	qsize = qa.size();

	for (i=0; i<qsize; i++) {
		
		curr = new Node;
		curr->blocked = qa.front()->blocked;
		curr->xpos = qa.front()->xpos;
		curr->ypos = qa.front()->ypos;
		curr->parent = qa.front()->parent;
		curr->h = qa.front()->h;
		curr->pc = qa.front()->pc;
		
		if ((curr->pc + curr->h) < bestd) {
			bestd = curr->pc+curr->h;
			best = curr;
		}
		
		qa.pop();
		qa.push(curr);
	}

	for (i=0; i<qsize; i++) {

		curr = new Node;
		curr->blocked = qa.front()->blocked;
		curr->xpos = qa.front()->xpos;
		curr->ypos = qa.front()->ypos;
		curr->parent = qa.front()->parent;
		curr->h = qa.front()->h;
		curr->pc = qa.front()->pc;
		
		if (curr->xpos == best->xpos && curr->ypos == best->ypos) {
			pop = new Node;
			pop->blocked = best->blocked;
			pop->xpos = best->xpos;
			pop->ypos = best->ypos;
			pop->parent = best->parent;
			pop->h = best->h;
			pop->pc = best->pc;
			qa.pop();
			return pop;
		}	

		qa.pop();
		qa.push(curr);
	}

	return NULL;
}

int astar(Node *n, int x, int y) {

	Node *m, *top, *left, *bottom, *right, *next;

	if (flag2==1)
		return 1;

	m = qa.front();

	if (m->ypos<24) {
		top = addtoqa(m,m->xpos,m->ypos+1);
		if (top) {
			top->h = manD(top, x, y);
			top->pc = pathCost(top);
		}
	}
	if (m->xpos<24) {
		right = addtoqa(m,m->xpos+1,m->ypos);
		if (right) {
			right->h = manD(right, x, y);
			right->pc = pathCost(right);
		}
	}
	if (m->ypos>0) {
		bottom = addtoqa(m,m->xpos,m->ypos-1);
		if (bottom) {
			bottom->h = manD(bottom, x, y);
			bottom->pc = pathCost(bottom);
		}
	}
	if (m->xpos>0) {
		left = addtoqa(m,m->xpos-1,m->ypos);
		if (left) {
			left->h = manD(left, x, y);
			left->pc = pathCost(left);
		}
	}

	next = searchqastar();

	if (next->xpos == x && next->ypos == y) {
		flag2 = 1;
		printf("\nFOUND\n");
		fin = next;
		return 1;
	}
	
	if (next) astar(next, x, y);

	return 0;
}

void createmaze() {
    
	FILE * pFile;
	pFile = fopen ("maze.txt","r");
	int count = 0;

	int d;
	int row=24;
	int column=0;
	
	for(;row>=0;row--) {
		for(;column<=24;column++) {
			fscanf(pFile,"%d",&d);
			newmaze[row][column] = d;
			if (d == 0)
				count++;
			printf("%d ",newmaze[row][column]);
		}
		printf("\n");
		column =0 ;
	}
	//printf("\n count of unblocked: %d \n",count);
	//getchar();
}

int main() {
	int startingx = 2;
	int startingy = 11;
	int goalx = 23;
	int goaly = 19;
	Node * start;

	createmaze();
	printf("\nMaze Read Successfully \n");
	printf("\nBeginning DFS.. press any key to continue \n");
	getchar();

	start = addtoq(NULL,startingx,startingy);
	DFS(start,goalx,goaly);
	
	if (flag == 1)
		printf("\nDFS SUCCESS :)\n");
	else
		printf("\nDFS FAIL :(\n");

	printf("\nDFS Path traversed: \n");

	while (fin->parent!=NULL) {
		printf("%d, %d \n", fin->xpos, fin->ypos);
		fin = fin->parent;
	}

	//printf("\n\nq size %d\n\n:)",q.size());
	printf("\nBeginning BFS.. press any key to continue \n");
	getchar();

	addtoqo(NULL,startingx,startingy);

	if (BFS(goalx,goaly) == 1)
		printf("\nBFS SUCCESS :)");
	else
		printf("\nBFS FAIL :(");

	printf("\nBFS Path traversed: \n");

	//printf("\n\nclosed q size %d\n\n:)",qc.size());
	//printf("\n\nopen q size %d\n\n:)",qo.size());

	while (fin->parent!=NULL) {
		printf("%d, %d \n", fin->xpos, fin->ypos);
		fin = fin->parent;
	}

	printf("\nBeginning ASTAR.. press any key to continue \n");
	getchar();

	start = addtoqa(NULL,startingx,startingy);
	astar(start,goalx,goaly);
	
	if (flag2 == 1)
		printf("\nASTAR SUCCESS :)\n");
	else
		printf("\nASTAR FAIL :(\n");

	printf("\nASTAR Path traversed: \n");

	while (fin->parent!=NULL) {
		printf("%d, %d \n", fin->xpos, fin->ypos);
		fin = fin->parent;
	}
	
	printf("\nDone.. press any key to exit \n");
	getchar();

	return 0;
}
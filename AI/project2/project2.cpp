
#include "project2.h"

square * loc[4][4];
int p = 1;
int depth = 1;
bool mflag = 0;
int alpha = -999;
int beta = 999;

#define dul = 1;
#define up = 2;
#define dur = 3;
#define right = 4;
#define ddr = 5;
#define down = 6;
#define ddl = 7;
#define left = 8;

game * creategame() {

	game * g = new game;
	square * n;
	int i,j;
	
	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			n = new square;
			n->xpos = i+1;
			n->ypos = j+1;
			n->c = 0;
			loc[i][j] = n;
			if (i==0 && j==3) { 
				n->s = 10;
				n->occby = 1;
			}
			else if (i==3 && j==0){
				n->s = 10;
				n->occby = 2;
			}
			else {
				n->s = 0;
				n->occby = 0;
			}

			g->que.push(n);
		}
	}

	g->parent = NULL;
	g->child = NULL;

	return  g;
}

game * copygame(game * g) {

	game * g2 = new game;
	square * n;
	int i,j;

	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			n = new square;
			n->xpos = loc[i][j]->xpos;
			n->ypos = loc[i][j]->ypos;
			n->c = 0;
			n->s = loc[i][j]->s;
			n->occby = loc[i][j]->occby;

			g2->que.push(n);
		}
	}

	g2->parent = g;
	//g->child = g2;

	return  g2;
}

void clear() {
	int i,j;
	square * n;

	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			n = loc[i][j];
			n->c = 0;
		}
	}
}

void updatelocator(game * g) {

	int i,j;
	square * n, * m;

	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			n = new square;
			m = g->que.front();
			n->xpos = m->xpos;
			n->ypos = m->ypos;
			n->c = m->c;
			n->s = m->s;
			n->occby = m->occby;

			loc[i][j] = n;

			g->que.pop();
			g->que.push(n);
		}
	}
}

void printgame() {

	int i,j;
	square * n;

	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			n = loc[i][j];
			printf("\nSquare (%d,%d) has %d stones occupied by Player %d",n->xpos,n->ypos,n->s,n->occby);
		}
		printf("\n");
	}
}

int move (square * start, int dir) {

	square * n, * pr;
	int x,y,s,i;

	n = pr = start;
	x = n->xpos;
	y = n->ypos;
	s = n->s;
	i = 0;

	if ((n->occby != p) || (n->occby == 0)) 
		return 0;

	if (dir == 1) {
		
		while (i<3 && x>1 && y<4 && s>0) {
			x = x - 1;
			y = y + 1;
			n = loc[x-1][y-1];

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else {
				mflag = 1;
				clear();
				if (i==0) {
					n->s = n->s + s;
					n->occby = p;
					pr->s = 0;
					pr->occby = 0;
				}
				else if (i==1) {
					n->s = n->s + s - 1;
					pr->s = pr->s - (s - 1);
					if ((s - 1)>0)
						n->occby = pr->occby;
				}
				else {
					n->s = n->s + s - 3;
					pr->s = pr->s - (s - 3);
					if ((s - 3)>0)
						n->occby = pr->occby;
				}
			}
			i++;
			pr = n;
		}
	}
	else if (dir == 2) {

		while (i<3 && y<4 && s>0) {
			x = x;
			y = y + 1;
			n = loc[x-1][y-1];

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else {
				mflag = 1;
				clear();
				if (i==0) {
					n->s = n->s + s;
					n->occby = p;
					pr->s = 0;
					pr->occby = 0;
				}
				else if (i==1) {
					n->s = n->s + s - 1;
					pr->s = pr->s - (s - 1);
					if ((s - 1)>0)
						n->occby = pr->occby;
				}
				else {
					n->s = n->s + s - 3;
					pr->s = pr->s - (s - 3);
					if ((s - 3)>0)
						n->occby = pr->occby;
				}
			}
			i++;
			pr = n;
		}

	}
	else if (dir == 3) {

		while (i<3 && x<4 && y<4 && s>0) {
			x = x + 1;
			y = y + 1;
			n = loc[x-1][y-1];

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else {
				mflag = 1;
				clear();
				if (i==0) {
					n->s = n->s + s;
					n->occby = p;
					pr->s = 0;
					pr->occby = 0;
				}
				else if (i==1) {
					n->s = n->s + s - 1;
					pr->s = pr->s - (s - 1);
					if ((s - 1)>0)
						n->occby = pr->occby;
				}
				else {
					n->s = n->s + s - 3;
					pr->s = pr->s - (s - 3);
					if ((s - 3)>0)
						n->occby = pr->occby;
				}
			}
			i++;
			pr = n;
		}

	}
	else if (dir == 4) {

		while (i<3 && x<4 && s>0) {
			x = x + 1;
			y = y;
			n = loc[x-1][y-1];

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else {
				mflag = 1;
				clear();
				if (i==0) {
					n->s = n->s + s;
					n->occby = p;
					pr->s = 0;
					pr->occby = 0;
				}
				else if (i==1) {
					n->s = n->s + s - 1;
					pr->s = pr->s - (s - 1);
					if ((s - 1)>0)
						n->occby = pr->occby;
				}
				else {
					n->s = n->s + s - 3;
					pr->s = pr->s - (s - 3);
					if ((s - 3)>0)
						n->occby = pr->occby;
				}
			}
			i++;
			pr = n;
		}
	}
	else if (dir == 5) {

		while (i<3 && x<4 && y>1 && s>0) {
			x = x + 1;
			y = y - 1;
			n = loc[x-1][y-1];

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else {
				mflag = 1;
				clear();
				if (i==0) {
					n->s = n->s + s;
					n->occby = p;
					pr->s = 0;
					pr->occby = 0;
				}
				else if (i==1) {
					n->s = n->s + s - 1;
					pr->s = pr->s - (s - 1);
					if ((s - 1)>0)
						n->occby = pr->occby;
				}
				else {
					n->s = n->s + s - 3;
					pr->s = pr->s - (s - 3);
					if ((s - 3)>0)
						n->occby = pr->occby;
				}
			}
			i++;
			pr = n;
		}
	}
	else if (dir == 6) {

		while (i<3 && y>1 && s>0) {
			x = x;
			y = y - 1;
			n = loc[x-1][y-1];

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else {
				mflag = 1;
				clear();
				if (i==0) {
					n->s = n->s + s;
					n->occby = p;
					pr->s = 0;
					pr->occby = 0;
				}
				else if (i==1) {
					n->s = n->s + s - 1;
					pr->s = pr->s - (s - 1);
					if ((s - 1)>0)
						n->occby = pr->occby;
				}
				else {
					n->s = n->s + s - 3;
					pr->s = pr->s - (s - 3);
					if ((s - 3)>0)
						n->occby = pr->occby;
				}
			}
			i++;
			pr = n;
		}
	}
	else if (dir == 7) {

		while (i<3 && x>1 && y>1 && s>0) {
			x = x - 1;
			y = y - 1;
			n = loc[x-1][y-1];

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else {
				mflag = 1;
				clear();
				if (i==0) {
					n->s = n->s + s;
					n->occby = p;
					pr->s = 0;
					pr->occby = 0;
				}
				else if (i==1) {
					n->s = n->s + s - 1;
					pr->s = pr->s - (s - 1);
					if ((s - 1)>0)
						n->occby = pr->occby;
				}
				else {
					n->s = n->s + s - 3;
					pr->s = pr->s - (s - 3);
					if ((s - 3)>0)
						n->occby = pr->occby;
				}
			}
			i++;
			pr = n;
		}
	}
	else if (dir == 8) {

		while (i<3 && x>1 && s>0) {
			x = x - 1;
			y = y;
			n = loc[x-1][y-1];

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else {
				mflag = 1;
				clear();
				if (i==0) {
					n->s = n->s + s;
					n->occby = p;
					pr->s = 0;
					pr->occby = 0;
				}
				else if (i==1) {
					n->s = n->s + s - 1;
					pr->s = pr->s - (s - 1);
					if ((s - 1)>0)
						n->occby = pr->occby;
				}
				else {
					n->s = n->s + s - 3;
					pr->s = pr->s - (s - 3);
					if ((s - 3)>0)
						n->occby = pr->occby;
				}
			}
			i++;
			pr = n;
		}
	}

	return 1;
}

int testmove (square * start, int dir) {

	square * n;
	int x,y;

	n = start;
	x = n->xpos;
	y = n->ypos;

	if ((n->occby != p) || (n->occby == 0)) 
		return 0;

	if (dir == 1) {
		
		if (x>1 && y<4) {
			x = x - 1;
			y = y + 1;
			n = loc[x-1][y-1];

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else
				return 1;
		}
		else return 0;
	}
	else if (dir == 2) {

		if (y<4) {
			x = x;
			y = y + 1;
			n = loc[x-1][y-1];		//this is not correct

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else
				return 1;
		}
		else
			return 0;
	}

	else if (dir == 3) {

		if (x<4 && y<4) {
			x = x + 1;
			y = y + 1;
			n = loc[x-1][y-1];		//this is not correct

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else
				return 1;
		}
		else
			return 0;
	}

	else if (dir == 4) {

		if (x<4) {
			x = x + 1;
			y = y;
			n = loc[x-1][y-1];		//this is not correct

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else 
				return 1;
		}
		else
			return 0;
	}
	else if (dir == 5) {

		if (x<4 && y>1) {
			x = x + 1;
			y = y - 1;
			n = loc[x-1][y-1];		//this is not correct

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else 
				return 1;
		}
		else
			return 0;
	}
	else if (dir == 6) {

		if (y>1) {
			x = x;
			y = y - 1;
			n = loc[x-1][y-1];		//this is not correct

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else 
				return 1;
		}
		else
			return 0;
	}
	else if (dir == 7) {

		if (x>1 && y>1) {
			x = x - 1;
			y = y - 1;
			n = loc[x-1][y-1];		//this is not correct

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else 
				return 1;
		}
		else
			return 0;
	}
	else if (dir == 8) {

		if (x>1) {
			x = x - 1;
			y = y;
			n = loc[x-1][y-1];		//this is not correct

			if ((n->occby != p) && (n->occby != 0))
				return 0;
			else 
				return 1;
		}
		else
			return 0;
	}
}

square * randmove(int &d) {

	int ri,rj,rdir;
	square * n;
	int t;

	t = 0;

	do {
		ri = rand() % 4;
		rj = rand() % 4;
		rdir = rand() % 8 + 1;

		n = loc[ri][rj];

		t = testmove(n,rdir);
	} while (t != 1);

	d = rdir;

	return n;
}

void evaluateh (game * g) {

	int i,j;
	int c1, c2;

	c1 = c2 = 0;

	for (i=0;i<4;i++) {
		for (j=0;j<4;j++) {
			if (loc[i][j]->occby == 1)
				c1++;
			else if (loc[i][j]->occby == 2)
				c2++;
		}
	}

	g->h = c1 - c2;
}


square * find() {
	int i,j;

	for (i=0;i<4;i++)
		for (j=0;j<4;j++)
			if (loc[i][j]->occby == p && loc[i][j]->c == 0) {
				//loc[i][j]->c = 1;
				return loc[i][j];
			}

	return NULL;
}

int pcount(game *g) {
	int i = 0;
	game * tmp;

	tmp = g;

	while (tmp->parent) {
		i++;
		tmp = tmp->parent;
	}

	return i;
}

int playgame(game * g) {

	square * n, *n2;
	game * g2, *best, *randm;
	int i;
	bool f1 = 0;

	best = randm = NULL;
	//depth--;

	while (depth<3) {
		
		n = find();
		f1 = 0;

		if (n!=NULL) {
			
			i = 0;

			while (i<8) {
				if (testmove(n,i+1) == 1)  {
					printgame();
					printf("\nPlaygame! Depth: %d\n",depth);
					//getchar(); 
					printf("\nSquare (%d,%d) has %d stones occupied by Player %d, about to move...\n",n->xpos,n->ypos,n->s,n->occby);
					//getchar();
					printf("\nMoving in direction: %d\n",i+1);
					getchar();

					g2 = copygame(g);
					updatelocator(g2);
					n2 = loc[n->xpos-1][n->ypos-1];
					move(n2,i+1);
					printf("\nAfter move...\n",i+1);
					printgame();
					//getchar();
					
					evaluateh(g2);

					if (p == 1 && g2->h > alpha) {
						alpha = g2->h;
						g->h = g2->h;
						best = g2;
						printf("\nAlpha is %d...\n",alpha);
						//getchar();
					}

					if (p == 2 && g2->h < beta) {
						beta = g2->h;
						g->h = g2->h;
						printf("\nBeta is %d...\n",beta);
						//getchar();
					}

					if (p == 2) randm = g2;
					
					if (f1==0) {
						depth++;
						f1 = 1;
					}

					if (depth<4) playgame(g2);
					else return 0;

					updatelocator(g);
				}

				i++;
			}
		
			updatelocator(g);
			depth--;
			loc[n->xpos-1][n->ypos-1]->c = 1;
			
		} else depth++;
	}	

	//depth--;

	if (p == 1) {
		if (best != NULL) {
			g->child = best;
			p = 2;
			alpha = -999;
			beta = 999;
			depth = 0;
			playgame(best);
			updatelocator(g);
		}
		else
			return 0;

	} else {
		if (randm != NULL) {
			g->child = randm;
			p = 1;
			alpha = -999;
			beta = 999;
			depth = 0;
			playgame(randm);
			updatelocator(g);
		}
		else
			return 0;
	}
}

int main() {
	
	game * g, *tmp;

	g = creategame();
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

	return 0;
}
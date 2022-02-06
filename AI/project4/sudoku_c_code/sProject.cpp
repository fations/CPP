// sProject.cpp : Defines the entry point for the console application.
..
#include "stdafx.h"
#include <iostream>
#include <ctime>

#include "LinkedList.h"
#include "Stack.h"
#include "String.h"

using namespace std;

//simple functions meant to be inline
#define Row(X) (X)/L
#define Col(X) (X)%L
#define Box(X) ((Row(X)/n)*n + Col(X)/n + 1)

//used for debugging
#define S1 0X1
#define S2 0X2
#define S3 0X4
#define S4 0X8
#define S5 0X10
#define S6 0X20
#define S7 0X40
#define S8 0X80
#define S9 0X100


//function prototypes
int isComplete(int *filled, int *no_candidates);
void fillAllCandidates(int *filled, int *no_candidates, int *candidates);
int rec_backtracking( int *filled, int *no_candidates, int *candidates, Stack *changes);

//global variables
int n = 3;
int L = n*n;
int N = L*L;


int main(int argc, char* argv[])
{
	char c;
	int i;

	//welcome screen
	cout << "\t***********************************" << endl;
	cout << "\t\tGPS+  Sudoku Solver" << endl;
	cout << "\t***********************************" << endl  << endl;

	//grid size

	cout << "Enter grid base size (eg: 3 for (3x3)^2 grid): ";
	cin >> n;
	while (n > 5 ) {
		cout << "In 32-bit machine maximum size allowed is 5\n Enter grid base size:";
		cin >> n; 
	}
	L = n*n;
	N = L*L;


	
	//allocating memory for system
	int *no_candidates = new int[N];
	int *filled = new int [N];
	for (i = 0; i < N; i++)
		no_candidates[i] = L+1;
	int *candidates = new int[N];
	Stack *changes = new Stack();

//debugging
//	int filled[] = {S9,0,0,S6,S5,S1,0,0,S8,0,0,0,0,0,S3,S6,0,0,0,S6,S4,S9,0,0,S7,0,0,S5,S4,0,S3,0,S2,S1,0,S6,S2,0,0,0,0,0,0,0,S4,S6,0,S1,S8,0,S7,0,S9,S2,0,0,S3,0,0,S9,S8,S6,0,0,0,S2,S5,0,0,0,0,0,S1,0,0,S7,S8,S4,0,0,S3};
//	int filled[] = {0,S4,S3,0,0,0,S6,0,0,0,0,0,S6,S9,0,0,0,S5,S8,0,0,0,0,S4,0,0,S7,0,0,S6,S7,0,S2,0,S9,0,0,S2,0,0,S3,0,0,S5,0,0,S1,0,S8,0,S6,S2,0,0,S1,0,0,S9,0,0,0,0,S4,S5,0,0,0,S7,S1,0,0,0,0,0,S2,0,0,0,S7,S8,0};
//	int filled[] = {S5,0,0,0,0,0,0,0,S8,0,0,0,S5,0,0,0,0,S9,S7,S9,S6,0,S4,0,S1,0,0,S9,0,0,S6,0,S4,0,S1,0,0,S7,0,0,0,0,0,S8,0,0,S3,0,S8,0,S1,0,0,S2,0,0,S1,0,S5,0,S9,S4,S7,S2,0,0,0,0,S7,0,0,0,S3,0,0,0,0,0,0,0,S6};
	
	
	//input	
	cout << endl << "Enter the puzzle \n(digits from 1 to (n x n)^2 used to denote value and 0 to denote empty cell)\n(enter a negative number to quit) :" << endl ;
	int temp;
	for (i = 0; i < N; i++) {
		cin >> temp;
		if (temp > 0)
			filled[i] = 1 <<(temp-1);
		else if(!temp)
			filled[i] = 0;
		else {
		    return (0);
			break;
		}
		no_candidates[i] = L+1;
	}

// program starts here
	int clock1 = clock();

	//initializing possible candidate stack - MRV
	fillAllCandidates(filled, no_candidates, candidates);
	// recursive backtracking
	rec_backtracking(filled, no_candidates, candidates, changes);

//program ends here
	int clock2 = clock();
	clock2 = clock2 - clock1;
	cout << endl<< endl<< "Puzzle solved in " <<  clock2 << ".. milli seconds" << endl <<endl;
	
	//output
	cout<< "Solution:";
	for (i = 0; i < N; i++)
		if (!filled[i]) {
			cout << "puzzle couldn't be solved";
			return 0;
		}

	for (int j = 0; j < N; j++)
		for (int k =1; k<=L; k++)
			if(1<<(k-1) & filled[j])
			{
				if (!(j%L))
					cout << endl;
				cout << k <<  " " ; //filled[j]<< " ";
			}

	//wait till user hits any key
	cin.read(&c,1);
	return 0;
}	

/*
 * function performing recursive backtracking based on CSP with MRV and forward checking
 *
 * @return
 *   -1 = failure
 *	 81 = success
 */
int rec_backtracking(int *filled, int *no_candidates, int *candidates, Stack *changes) {
	
	int t = isComplete(filled, no_candidates);
	if(t==N)
		return N;
	int c = no_candidates[t];


	for(int x=1; (x <= L) && c; x++) {
		int temp = 1<<(x-1);
		if (temp & candidates[t]) {
			c--;
			filled[t] = temp;

			//assignment			
			int i = 0; 
			int b = Row(t)*L;
			for(i= b; i< L+b; i++) {
				if (!filled[i]) {
					if (candidates[i]&temp) {
						candidates[i] &= (~temp);
						no_candidates[i]--;
						changes->Push(i);
					}
				}
			}
			b = Col(t);
			for(i = b; i< N; i= i+L) {
				if (!filled[i]) {
					if (candidates[i]&temp) {
						candidates[i] &=  (~temp);
						no_candidates[i]--;
						changes->Push(i);
					}
				}
			}
			b  = ((Box(t)-1)/n)*n*L + ((Box(t)-1)%n*n);
			for(i = b; i<b + n*L; i = i + L) {
				for(int k=0; k <n; k++) {
					if (!filled[k+i]) {
						if (candidates[k+i]&temp) {
							candidates[k+i] &=   (~temp);
							no_candidates[k+i]--;
							changes->Push(k+i);
						}
					}
				}
			}
			changes->Push(~t);
		
			if (rec_backtracking(filled,no_candidates,candidates,changes) != -1) {
				return N;
			}
			else
			{
				//removeAssignmnet
					int t = ~(changes->Pop());
					temp = filled[t];
					filled[t] = 0;
					while ( (t=changes->Pop()) > 0) {
						candidates[t] |= temp;
						no_candidates[t]++;
					}
					changes->Push(t);

			}
		}
	}
	return -1;
}


/*
 * checks for completeness of the puzzle and returns MRV
 *
 * @return
 *   0 - puzzle is completed
 *	 -1 - failure
 *   n - cell with minimum no of candidates
 */
int isComplete(int *filled, int *no_candidates) {
	int location = N;
	int minVal = L+1;
	for(int a = 0; a <N; a++) {
		if (!filled[a]) {
			if (minVal > no_candidates[a]) {
				minVal = no_candidates[a];
				location = a;
			}
		}
	}
	return location;
}


/*
 * parent funciton of fillCandidate, calls fillCandidate for every cell
 *
 */
void fillAllCandidates(int *filled, int *no_candidates, int *candidates) {
	for (int location = 0; location< N; location++) {
		if (!filled[location]) {
				int c = 0;
				int l = 0;
				int i = 0; 
				//	doing row
				int b = Row(location)*L;
				for(i= b; i< L+b; i++) {
					l |=  filled[i];
				
				}
				//	doing column
				b = Col(location);
				for(i = b; i< N; i= i+L) {
					l |=   filled[i];
				}

				// doing box
				b  = ((Box(location)-1)/n)*n*L + ((Box(location)-1)%n*n);
				for(i = b; i<b + n*L; i = i + L) {
					for(int k=0; k <n; k++) {
						l |= filled[k + i];
					}
				}

				l = ~l;
				l = l & ((1 << L)-1);
				for (i = 1; i <= L; i ++)
					if ( l & (1<<(i-1)))
						c++;

				no_candidates[location] = c;
				candidates[location] = l;
		}
	}
	return;
}

#include "PCB.h"

#ifndef _PCBQ_H
#define _PCBQ_H   

class PCBQ                  
{
	struct Node {
		PCB *x;
		Node *next;
		Node *prev;
	};  

	Node *node, 
		 *list_h, 
		 *list_t;
	
	public:
	
		PCBQ(void);
		~PCBQ(void);
		
		PCB *popQueueNode(void);
		void popTarget(PCB*);
		
		void addNodeAtFront(PCB*);
		void addNodeAtEnd(PCB*);
		
		int hasNodes(void);
		
		void showQueue(void);
		
};

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////

PCBQ::PCBQ (void)	
{
	list_h = list_t = node = NULL;						// set both head and tail to NULL 
}                   

///////////////////////////////////////////////////////////////////////////////////////////////////////

PCBQ::~PCBQ (void)								// destructor, dangerous: turn destructor into cleanup before implementing?
{												// called automatically in C++

	while (list_h)								// loop while head exists. If head doesnt exist then dont bother deleting
	{											//    anything since there is nothing to delete
		node = list_h;							// node gets assigned as head
		list_h = list_h->next;					// list_h gets address in ->next. Effectively increment head to next element 
		delete node;							// delete node 
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////

PCB* PCBQ::popQueueNode(void) //FIFO
{	
	PCB *toReturn; 
	
	if (list_h)						// check to see that there are elements in the queue 
	{
		node = list_h;				// set node to head of queue 

		list_h = list_h->next;		// set head of queue to next element in queue 
		
		if (!list_h)				// if the next element is NULL then there was only one element in the queue
			list_t = NULL;			// set the tail to NULL since there is only one element 
	
		else
			list_h->prev = NULL;
		
		toReturn = node->x;
		free(node);
		
		return toReturn;				// return the message envelope contained by the old head
	}
	
	return NULL;					// if there was no element in the queue to begin with return NULL
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void PCBQ::popTarget(PCB *target_PCB)
{
	Node *temp;
	bool flag;
	
	node = list_h;
	flag = false;								
	
	while (node && !flag){
			
		temp = node;
		
		if (node->x->getPID() == target_PCB->getPID()) {
		
			if (list_h == list_t)
				list_h = list_t = NULL;
			
			else if (node == list_h) {
				node->next->prev = temp->prev;
				list_h = temp->next;
			}
			
			else if (node == list_t) {
				node->prev->next = temp->next;
				list_t = temp->prev;
			}
			
			else {
				node->next->prev = temp->prev;
				node->prev->next = temp->next;
			}
			
			temp->next = temp->prev = NULL;	
			
			free(node);
			flag = true;	
		}		
			
		if (!flag)
			node = node->next;						// Increment node to next node
	}  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void PCBQ::addNodeAtFront(PCB *toAdd)		
{
	node = new Node;
	
	if (!list_h) {	
		node->x = toAdd;
		
		list_h = list_t = node;
		list_h->prev = list_t->next = NULL;
	}
	
	else {
		node->x = toAdd;
		node->next = list_h;            // new node points 'ahead' to list_h, head
		list_h->prev = node;			// head points 'back' to new node
		list_h = node;                  // head takes address of temp
		list_h->prev = NULL;			// head points 'back' to nothing, NULL
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void PCBQ::addNodeAtEnd(PCB *toAdd)
{
	Node *temp = new Node;
	
	if (list_t == NULL) {
		temp->x = toAdd;
		
		temp->next = temp->prev = NULL;
		list_h = list_t = temp;
	}
	
	else {
		temp->x = toAdd;
		temp->prev = list_t;		
		list_t->next = temp;
		list_t = temp;
		list_t->next = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int PCBQ::hasNodes (void)
{
  return (list_h != NULL);						// if a head exists, then elements exist in the queue, so return TRUE 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void PCBQ::showQueue (void)
{	
	node = list_h;
	
	while (node) {
		cout<<node->x->getPID()<<"\t"<<node->x->getState()<<"\n";
		node = node->next;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
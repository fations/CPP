#include <iostream.h>
#include "MsgEnv.h"

#ifndef _MsgEnvQ_H
#define _MsgEnvQ_H   

class MsgEnvQ                  
{
	struct Node {
		MsgEnv *x;
		Node *next;
		Node *prev;
	};  

	Node *node, 
		 *list_h, 
		 *list_t;
	
	public:
		MsgEnvQ(void);
		~MsgEnvQ(void);
		
		MsgEnv *popQueueNode(void);
		
		void addNodeAtFront(MsgEnv*);
		void addNodeAtEnd(MsgEnv*);
		
		int hasNodes(void);
		int getSize(void);
		
		void updateDelay(int);
		
		void showQueue(void);			
};

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////

MsgEnvQ::MsgEnvQ (void)	
{ list_h = list_t = node = NULL;}                   

///////////////////////////////////////////////////////////////////////////////////////////////////////

MsgEnvQ::~MsgEnvQ (void)			// destructor, dangerous: turn destructor into cleanup before implementing?
{									// called automatically in C++
	
	while (list_h)					// loop while head exists. If head doesnt exist then dont bother deleting
	{								//    anything since there is nothing to delete
		node = list_h;				// node gets address of head 
		list_h = list_h->next;		// list_h gets address in ->next. Effectively increment head to next element 
		delete node;				// delete node 
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////

MsgEnv *MsgEnvQ::popQueueNode(void) //FIFO
{	
	MsgEnv *toReturn;
	
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

void MsgEnvQ::addNodeAtFront(MsgEnv *toAdd)		
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

void MsgEnvQ::addNodeAtEnd(MsgEnv *toAdd)
{
	node = new Node;
	
	if (!list_t) {
		node->x = toAdd;
		
		list_h = list_t = node;
		list_h->prev = list_t->next = NULL;
	}
	
	else {
		node->x = toAdd;
		node->prev = list_t;
		list_t->next = node;
		list_t = node;
		list_t->next = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int MsgEnvQ::hasNodes (void)
{
	return (list_h != NULL);			// if a head exists, then elements exist in the queue, so return TRUE 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int MsgEnvQ::getSize(void)
{
	int i = 0;
	
	node = list_h;
	
	while (node) {
		i++;
		node = node->next;
	}
	
	return i;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void MsgEnvQ::updateDelay (int res)
{
	double prev_delay;
	double new_delay;
	char* message = new char [512];
	
	node = list_h;

	while (node) {
		prev_delay = atof(node->x->getMessage());
		new_delay = prev_delay - (double) res/1000000;
		snprintf(message, 512, "%lf", new_delay);
		node->x->setMessage(message);
		node = node->next;
	}
	
	delete(message);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void MsgEnvQ::showQueue (void)
{	
	node = list_h;
	
	while (node) {
		cout<<node->x->getMsgID()<<"\t"<<node->x->getPIDFrom()<<"\t"<<node->x->getPIDTo()<<"\t"<<node->x->getMessageType()<<"\t"<<node->x->getMessage()<<"\n";
		node = node->next;
	}
	
	cout<<"\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
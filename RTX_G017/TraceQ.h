#include "Trace.h"

#ifndef _TraceQ_H
#define _TraceQ_H   

class TraceQ                  
{
	struct Node
	{
		Trace *x;
		Node *next;
		Node *prev;
	};  

	Node *node, 
		 *list_h, 
		 *list_t;
	
	public:
		TraceQ (void);
		~TraceQ (void);
		
		Trace *popQueueNode (void);
		Trace *popStackNode(void);
		
		void addNodeAtFront (Trace *toAdd);
		void addNodeAtEnd (Trace *toAdd);
		
		int hasNodes (void);
		void sendTrace(char *message);
		void receiveTrace(char *message);
};

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////

TraceQ::TraceQ (void)	
{
  list_h = list_t = node = NULL;			// set both head and tail to NULL 
}                   

///////////////////////////////////////////////////////////////////////////////////////////////////////

TraceQ::~TraceQ (void)				// destructor, dangerous: turn destructor into cleanup before implementing?
{									// called automatically in C++	
	while (list_h)					// loop while head exists. If head doesnt exist then dont bother deleting
	{								//    anything since there is nothing to delete
		node = list_h;				// node gets address of head 
		list_h = list_h->next;		// list_h gets address in ->next. Effectively increment head to next element 
		delete node;				// delete node 
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////

Trace *TraceQ::popQueueNode(void) //FIFO
{	
	Trace *toReturn;
	
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

Trace *TraceQ::popStackNode(void) //FIFO
{	
	Trace *toReturn;
	
	if (list_t)						// check to see that there are elements in the queue 
	{
		node = list_t;				// set node to head of queue 

		list_t = list_t->prev;		// set head of queue to next element in queue 
		
		if (!list_t)				// if the next element is NULL then there was only one element in the queue
			list_h = NULL;			// set the tail to NULL since there is only one element 
	
		else
			list_t->next = NULL;
		
		toReturn = node->x;
		
		free(node);
		return toReturn;				// return the message envelope contained by the old head
	}
	
	return NULL;					// if there was no element in the queue to begin with return NULL
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void TraceQ::addNodeAtFront(Trace *toAdd)		
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

void TraceQ::addNodeAtEnd(Trace *toAdd)
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

void TraceQ::sendTrace(char *message)
{	
	char temp_SourcePID[8];								
	char temp_DestPID[8];
	char temp_TimeStamp[8];
	char temp_MsgType[8];
	char temp_MsgID[3];
	char temp1[2] = {'\t','\0'};
	char temp2[2] = {'\n','\0'};

	int SourcePID = 0;									
	int DestPID; 
	int TimeStamp;
	int MsgType;
	int MsgID;
	int i = 0;
	int flag = 0;
	
	node = list_h;
	
	while (node && (flag == 0))						
	{
		SourcePID = node->x->getSourcePID();			
		
		flag = 1;
		
		if (SourcePID != -1)
		{
			flag = 0;
			snprintf (temp_SourcePID, 8, "%d", SourcePID);
			DestPID = node->x->getDestPID();
			snprintf (temp_DestPID, 8, "%d", DestPID);
			TimeStamp = node->x->getTimeStamp();
			snprintf (temp_TimeStamp, 8, "%d", TimeStamp);
			MsgType = node->x->getMsgType();
			snprintf (temp_MsgType, 8, "%d", MsgType);
			MsgID = node->x->getMsgID();
			snprintf (temp_MsgID, 3, "%d", MsgID);
			
			strcat(message,temp_MsgID);
			strcat(message,temp1);	
			strcat(message,temp_SourcePID);				// Concatenate temp strings to message string
			strcat(message,temp1);
			strcat(message,temp_DestPID);
			strcat(message,temp1);
			strcat(message,temp_TimeStamp);
			strcat(message,temp1);
			strcat(message,temp_MsgType);
			strcat(message,temp2);
		}
		
		else if ((i == 0) && (flag == 1))
			strcpy (message,"\nNo Elements could be found in Send trace queue\n\0");
		
		node = node->next;
		i++;
	}	
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void TraceQ::receiveTrace(char *message)
{	
	char temp_SourcePID[8];								
	char temp_DestPID[8];
	char temp_TimeStamp[8];
	char temp_MsgType[8];
	char temp_MsgID[3];
	char temp1[2] = {'\t','\0'};
	char temp2[2] = {'\n','\0'};

	int SourcePID = 0;									
	int DestPID; 
	int TimeStamp;
	int MsgType;
	int MsgID;
	int i = 0;
	int flag = 0;
	
	node = list_h;
	
	while (node && (flag == 0))						
	{
		SourcePID = node->x->getSourcePID();			
		
		flag = 1;
		
		if (SourcePID != -1)
		{
			flag = 0;
			snprintf (temp_SourcePID, 8, "%d", SourcePID);
			DestPID = node->x->getDestPID();
			snprintf (temp_DestPID, 8, "%d", DestPID);
			TimeStamp = node->x->getTimeStamp();
			snprintf (temp_TimeStamp, 8, "%d", TimeStamp);
			MsgType = node->x->getMsgType();
			snprintf (temp_MsgType, 8, "%d", MsgType);
			MsgID = node->x->getMsgID();
			snprintf (temp_MsgID, 3, "%d", MsgID);
			
			strcat(message,temp_MsgID);
			strcat(message,temp1);
			strcat(message,temp_SourcePID);				// Concatenate temp strings to message string
			strcat(message,temp1);
			strcat(message,temp_DestPID);
			strcat(message,temp1);
			strcat(message,temp_TimeStamp);
			strcat(message,temp1);
			strcat(message,temp_MsgType);
			strcat(message,temp2);
		}
		
		else if ((i == 0) && (flag == 1))
			strcpy (message,"\nNo Elements could be found in Receive trace queue\n\0");
		
		node = node->next;
		i++;
	}	
																																																			
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int TraceQ::hasNodes (void)
{
	return (list_h != NULL);			// if a head exists, then elements exist in the queue, so return TRUE 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
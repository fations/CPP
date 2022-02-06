// LinkedList.cpp: implementation of the LinkedList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LinkedList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


LinkedList::LinkedList()
{
	current=NULL;
	first=NULL;
	length=0;
}


LinkedList::LinkedList(LinkedList &linkedList)
{
	current=NULL;
	first=NULL;
	length=0;

	int length=linkedList.GetLength();
	for (int i=0;i<length;i++)
	{
		Insert(linkedList.GetNext());
	}
}



LinkedList::~LinkedList()
{
	while (length!=0)
	{
		Remove();
	}
}


void LinkedList::Insert(int val)
{
	_node *p_node=new _node;
	p_node->value=val;

	p_node->next=first;
	first=p_node;
	current = first;
	length++;
//	current =first;
}


void LinkedList::Remove()
{
	if (length>0)
	{
		_node *tmp=first;
		first=first->next;
		delete tmp;
		length--;
	}
//	current = first;
}


void LinkedList::InsertAfter(int val1,int val2)
{

	_node *pn=new _node;
	_node *pextra;
	_node *inter;
	pn->value=val2;
	current=first;

	if ((first->value)==val1)
	{
		pn->next=first;
		first=pn;
		current=first;
		length++;
	}
	else
	{

		int k=this->GetLength();
		pextra=first;
		current=pextra->next;
		for (int q=0;q<k-1;q++)
		{
		
			inter=current;	
			int x=this->GetNext();
			
			if (x==val1)
			{
				pextra->next=pn;
				pn->next=inter;
				inter=current;
				length++;
				current=first;
				break;
			}
			pextra=pextra->next;
			inter=inter->next;
		
		}
	}
current = first;
	
}


bool LinkedList::Remove(int val)
{
	_node *last;
		
	int k = this->GetLength();
	if(!k)
		return false;
	last = first;
	if (first->value == val){
		first = first->next;
		delete last;
		length--;
		current = first;
		return true;
	}
	current=first->next;
	for (int i=0;i<k-1;i++)
	{
		if(current->value == val) {
			last->next = current->next;
			delete current;
			length--;
			current = first;
			return true;
		}
		else {
			last = current;
			current = current->next;
		}
	}
	current = first;
	return false;
}


int LinkedList::GetNext()
{
	int val=-1;

	if (length>0)
	{
		if (current==NULL)
		{
			current=first;
		}

		val=current->value;
		current=current->next;
	}

	return val;
}


int LinkedList::GetLength()
{
	return length;
}


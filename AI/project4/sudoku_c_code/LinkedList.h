// LinkedList.h: interface for the LinkedList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINKEDLIST_H__C7CBFDE5_01BD_43B0_A657_76A6B1F87CAF__INCLUDED_)
#define AFX_LINKEDLIST_H__C7CBFDE5_01BD_43B0_A657_76A6B1F87CAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class LinkedList  
{
	struct _node
	{
		int value;
		_node *next;
	} ;
private:
	
	
	_node *first;
	_node *current;
	int length;

public:
	int GetLength();
	int GetNext();
	bool Remove(int val);
	void Insert(int val);
	void InsertAfter(int val1,int val2);
	void Remove();

	LinkedList();
	LinkedList(LinkedList &linkedLIst);
	virtual ~LinkedList();

};

#endif // !defined(AFX_LINKEDLIST_H__C7CBFDE5_01BD_43B0_A657_76A6B1F87CAF__INCLUDED_)

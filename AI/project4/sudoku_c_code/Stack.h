// Stack.h: interface for the Stack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STACK_H__D40AC1D7_58FA_4507_8534_1A877C7052B7__INCLUDED_)
#define AFX_STACK_H__D40AC1D7_58FA_4507_8534_1A877C7052B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LinkedList.h"


class Stack  
{
	friend class Stack;

private:
	LinkedList list;
public:
	Stack();
	virtual ~Stack();
	Stack(Stack& St);
	void Push(int);
	bool del(int);
	int Pop();


};

#endif // !defined(AFX_STACK_H__D40AC1D7_58FA_4507_8534_1A877C7052B7__INCLUDED_)

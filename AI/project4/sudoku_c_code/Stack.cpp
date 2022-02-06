 // Stack.cpp: implementation of the Stack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Stack.h"
#include "iostream.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


Stack::Stack()
{
//	cout << "Stack is created......." << endl;
}

Stack::~Stack()
{
	while(list.GetLength() !=0)		//Remove everything from stack
			Pop();
	//cout << "Stack is Destroyed......." << endl;
}

Stack::Stack(Stack& St)
{	
	cout << "Copy constructor" << endl;
	for(int i=0; i < St.list.GetLength(); i++)
		Push(St.list.GetNext());
}

void Stack::Push(int num)
{

	list.Insert(num);

}
bool  Stack::del(int val) {
    return list.Remove(val);
}


int Stack::Pop()					//pop the value
{
	int temp;
	if(list.GetLength()==0)			//check for empty stack
	{
		cout << "Stack is empty................" << endl;
		return -1;
	}

	temp=list.GetNext();			//read the value
	list.Remove();					//remove the value
	return temp;
}

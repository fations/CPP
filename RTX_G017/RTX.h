#include <string.h>
#include "PCBQ.h"
#include "TraceQ.h"
#include "shm.h"
#include "gtime.h"

using namespace std;
#define STACKSIZE 65536
#define MAX_ENV 10
#define res 100000

extern "C" {void _set_sp(char * );}

///////////////////////////////////////////////////////////////////////////////////////////////////////

	MsgEnv *MsgEnv_toAdd = new MsgEnv[MAX_ENV];
	MsgEnv *MsgEnv_iProc = new MsgEnv[2];
	
	Trace *Trace_toAdd = new Trace[32];

	MsgEnvQ *FreeQ = new MsgEnvQ;
	TraceQ *ReceiveQ = new TraceQ;
	TraceQ *SendQ = new TraceQ;
	
	MsgEnvQ *TimeQ = new MsgEnvQ;
	
	PCB *PCBArray = new PCB[9];
	
	PCBQ *RPQArray = new PCBQ[4];
	PCBQ *BORArray = new PCBQ[4];
	PCBQ *BOEArray = new PCBQ[4];
	
	PCB *current_process;
	
	smStruct *shm1, *shm2;
	int shmid1, shmid2;
    key_t key1, key2;
	int rxpid, txpid;
	
	g_time timeglobal;
	
	unsigned long int ticks = 0;
	
///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RTX_H
#define _RTX_H

class RTX
{
	public: 
	
		void initialize (void);
		void terminate (void);
		int send_message (int dest_PID, MsgEnv *toSend);
		MsgEnv *receive_message (void);
		int release_msg_env (MsgEnv *env);
		MsgEnv *request_msg_env (void);
		int release_processor (void);
		int request_process_status (MsgEnv *toFill);
		int change_priority (int new_priority, int target_PID);
		int get_trace_buffers (MsgEnv *toFill);
		int send_console_chars(MsgEnv *env);
		int get_console_chars(MsgEnv *env);
		int request_delay (int delay, int code, MsgEnv *env);
		
} rtx;

#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////

char *get_time (void)
{
	char sec[4];
	char min[4];
	char hr[4];
	char sep[2] = {':','\0'};
	char sep2[1] = {'\0'};
	char time[32];
		
	memset(time,'\0',32);
	
	snprintf (sec, 4, "%d", timeglobal.sec);				
	snprintf (min, 4, "%d", timeglobal.min);				
	snprintf (hr, 4, "%d", timeglobal.hr);				
	
	if (timeglobal.hr <=9) {
		char *zero = new char [3];
		memset(zero,'\0',3);
		zero[0] = '0';
		strcat (zero,hr);
		strcat(time,zero);
		strcat(time,sep);
	}
		
	else {
		strcat(time,hr);
		strcat(time,sep);
	}
	
	if (timeglobal.min <=9) {
		char *zero = new char [3];
		memset(zero,'\0',3);
		zero[0] = '0';
		strcat (zero,min);
		strcat(time,zero);
		strcat(time,sep);
	}
	
	else {
		strcat(time,min);
		strcat(time,sep);
	}
		
	if (timeglobal.sec <=9) {
		char *zero = new char [3];
		memset(zero,'\0',3);
		zero[0] = '0';
		strcat (zero,sec);
		strcat(time,zero);
		strcat(time,sep2);
	}
	
	else {
		strcat(time,sec);
		strcat(time,sep2);
	}

	return time;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

char *format_time (void)
{
	char sec[4];
	char min[4];
	char hr[4];
	char sep[2] = {':','\0'};
	char sep2[1] = {'\0'};
	char time[32];
	
	memset(time,'\0',32);
	
	timeglobal.sec++;
			
	if (timeglobal.sec == 60)
	{
		timeglobal.sec = 0;
		timeglobal.min ++;
		
		if (timeglobal.min == 60)
		{
			timeglobal.min = 0;
			timeglobal.hr ++;
				
			if (timeglobal.hr == 24)
			{
				timeglobal.hr = 0;
				timeglobal.min = 0;
				timeglobal.sec = 0;
			}
		}
	}
		
	snprintf (sec, 4, "%d", timeglobal.sec);				
	snprintf (min, 4, "%d", timeglobal.min);				
	snprintf (hr, 4, "%d", timeglobal.hr);				
	
	if (timeglobal.hr <=9) {
		char *zero = new char [3];
		memset(zero,'\0',3);
		zero[0] = '0';
		strcat (zero,hr);
		strcat(time,zero);
		strcat(time,sep);
	}
		
	else {
		strcat(time,hr);
		strcat(time,sep);
	}
	
	if (timeglobal.min <=9) {
		char *zero = new char [3];
		memset(zero,'\0',3);
		zero[0] = '0';
		strcat (zero,min);
		strcat(time,zero);
		strcat(time,sep);
	}
	
	else {
		strcat(time,min);
		strcat(time,sep);
	}
		
	if (timeglobal.sec <=9) {
		char *zero = new char [3];
		memset(zero,'\0',3);
		zero[0] = '0';
		strcat (zero,sec);
		strcat(time,zero);
		strcat(time,sep2);
	}
	
	else {
		strcat(time,sec);
		strcat(time,sep2);
	}

	return time;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void atomic(bool flag) {
	
	static sigset_t oldmask;
	sigset_t newmask;
	
	if (flag) {
		current_process->incrAtomicCount();

		if (current_process->getAtomicCount() == 1) {
			sigemptyset(&newmask);
			sigaddset(&newmask, 14);
			sigaddset(&newmask, 2);
			sigaddset(&newmask, SIGUSR1);
			sigaddset(&newmask, SIGUSR2);
			sigprocmask(SIG_BLOCK, &newmask, &oldmask);
		}
	} 
	
	else {
		current_process->decrAtomicCount();
		
		if (current_process->getAtomicCount() == 0)
			sigprocmask(SIG_SETMASK, &oldmask, NULL);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void swapTo(PCB *next_process)
{
	atomic(true);

    int return_code;
    
    return_code = setjmp(current_process->context);
    
    if (return_code == 0) {
    
		current_process->decrAtomicCount();
		current_process = next_process;
        current_process->incrAtomicCount();
        current_process->setState(0);
    
        atomic(false);
        longjmp(current_process->context, 1);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void process_switch(void) {

	atomic(true);

	int state;
	int priority;
	int pid;
	PCBQ* pcbqPtr;
	PCB* pcbPtr;
	int i;
	
	pid = current_process->getPID();
	state = current_process->getState();
	priority = current_process->getPriority();
	
	if (state==1)
		RPQArray[priority].addNodeAtEnd(&PCBArray[pid]);
	else if (state==2)
		BORArray[priority].addNodeAtEnd(&PCBArray[pid]);
	else if (state==3)
		BOEArray[priority].addNodeAtEnd(&PCBArray[pid]);
	
	i = 0;
	
	do {													
		pcbqPtr = &RPQArray[i];
		i++;	
	} while ((!pcbqPtr->hasNodes()) && (i < 4));

	pcbPtr = pcbqPtr->popQueueNode();

	atomic(false);
	swapTo(pcbPtr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void initialize_PCB (PCB &new_PCB) {

    jmp_buf temp;

    new_PCB.stack = (long *) malloc(STACKSIZE);
	
    if (setjmp(temp) == 0) { 
	   
        _set_sp((char *)new_PCB.stack + STACKSIZE);            
    
		if (setjmp(new_PCB.context) == 0) {
        
			longjmp(temp, 1);
        } 
		
		else {
            PCBArray[current_process->getPID()].start();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int RTX::send_message (int dest_PID, MsgEnv *toSend)
{
	atomic(true);
	
	Trace *toAdd;
	PCB *dest_PCB;
   
	if (toSend)												// Continue only if a valid message envelope was 
	{														//    provided 
		
		dest_PCB = &PCBArray[dest_PID];						// Obtain PCB pointer from PCBArray using PID
		
		toSend->setPIDFrom(current_process->getPID());		// Update message envelop fields 
		toSend->setPIDTo(dest_PID);
		toSend->setTime(ticks);							// Define Global Time variable!!!

		dest_PCB->getMessageEnvQueue()->addNodeAtEnd(toSend);							//    message envelop queue

		if ((dest_PCB->getState()) == 2)						// Check to see if destination process was BOR
		{		
			BORArray[dest_PCB->getPriority()].popTarget(dest_PCB); // Remove process off respective BOR Queue
			dest_PCB->setState(1);							// Set process status to ready
			RPQArray[dest_PCB->getPriority()].				// Throw process onto respective RPQ
				addNodeAtEnd(dest_PCB);

		}
		
		toAdd = SendQ->popStackNode();						// Get Trace element from SendQ. Either expired
															//    or unfilled element. ALWAYS pop back onto queue
		toAdd->setSourcePID (toSend->getPIDFrom());			// Copy message envelope fields into Trace element
		toAdd->setDestPID (toSend->getPIDTo());
		toAdd->setTimeStamp (toSend->getTime());
		toAdd->setMsgType (toSend->getMessageType());
		toAdd->setMsgID (toSend->getMsgID());
		
		SendQ->addNodeAtFront(toAdd);							// Throw trace back onto SendQ 
		
		atomic(false);
		return 0;											// Return success 
	}
   
	atomic(false);
	return 1;												// Return error code 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

MsgEnv *RTX::receive_message (void)
{
	atomic(true);

	Trace *toAdd = NULL;
	MsgEnv *toReturn = NULL;
	int pid;
		
	if (current_process->getMessageEnvQueue()->hasNodes())	// Continue only if message envelope is available
	{	
		toReturn = current_process->getMessageEnvQueue()->popQueueNode();			//    message envelope queue
					
		toReturn->setTime(ticks);							// Necessary???
		
		toAdd = ReceiveQ->popStackNode();					// Get Trace element from ReceiveQ. Either expired
															//    or unfilled element. ALWAYS pop back onto queue		
		toAdd->setSourcePID (toReturn->getPIDFrom());		// Copy message envelope fields into Trace element
		toAdd->setDestPID (toReturn->getPIDTo());
		toAdd->setTimeStamp (toReturn->getTime());
		toAdd->setMsgType (toReturn->getMessageType());
		toAdd->setMsgID (toReturn->getMsgID());
						
		ReceiveQ->addNodeAtFront(toAdd);					// Throw trace back onto ReceiveQ	
		
		atomic(false);							
		return (toReturn);									// Return message envelope from current process' 
	}														//    message envelope queue
	
	else													// Unavailale message envelope on PCB's queue
	{														
		pid = current_process->getPID();				
	
		if ((pid == 1) || (pid == 2) || (pid == 3))	{
			atomic(false);
			return NULL;									// iprocess never blocks so return NULL
		}
			
		else {			
			current_process->setState(2);					// Set current process' status to BOR
			
			atomic(false);
			process_switch();								// Conduct process switch
			return NULL;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int RTX::release_msg_env (MsgEnv *env)
{
	atomic(true);
	
	PCBQ *PCBQ_ptr = NULL;									// Assume no BOE processes in queue
	PCB *PCB_ptr;												
	int i = 0;

	if (env)												// Continue only if message envelope is valid
	{	
		env->clear();										// Clear contents of message envelope
		
		do {													// Find highest priority BOE process													
			PCBQ_ptr = &BOEArray[i];						// If a BOE process is found set pointer to it
			i++;	
		} while ((!PCBQ_ptr->hasNodes()) && (i < 4));
		
		if (PCBQ_ptr->hasNodes())							// If a BOE process was found then continue
		{
			PCB_ptr = PCBQ_ptr->popQueueNode();				// Get the first BOE process in the BOE queue	
			
			PCB_ptr->setState(1);							// Set process status to ready
			RPQArray[PCB_ptr->getPriority()].addNodeAtEnd(PCB_ptr);	
												// Return success code
		}
		
		FreeQ->addNodeAtEnd(env);

		atomic(false);
		return 0;	
	}
	
	atomic(false);	
	return 1;												// Invalid message envelope, return error
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

MsgEnv *RTX::request_msg_env (void)
{
	atomic(true);
	
	MsgEnv *toReturn;
	int PID;
	
	PID = current_process->getPID();					

	toReturn = FreeQ->popQueueNode();						// Pop free message envelope off freeMsgQ
	
	if (toReturn) {
		atomic(false);
		return toReturn;									// Return message envelope
	}
		
	else {
		current_process->setState(3);					// Set current process' status to BOE	
		atomic(false);
		process_switch ();								// Conduct process switch
		return NULL;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int RTX::release_processor (void)
{
    atomic(true);
	
	current_process->setState(1);							// Set Current process' status to ready
	
	atomic(false);
	process_switch();										// Conduct process switch
	
	return 0;												// Return success code
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int RTX::request_process_status (MsgEnv *toFill)
{
	atomic(true);
	
	PCB *PCB_ptr;											// Pointer to PCB
	char message[512];										// String to hold message to be sent to console
	char temp_PID[8];										// Temp strings to hold integer converted values
	char temp_State[8];										
	char temp_Priority[8];								
	char temp1[2] = {'\t','\0'};											
	char temp2[2] = {'\n','\0'};
	char header[32];
	
	message[0] = '\0';
	
	int PID, State, Priority;							
	
	if (toFill)												// Only continue if valid message envelope was provided
	{
		strcpy(header,"\n\npid\tstate\tpriority\n");
		strcat(message,header);
		
		for (int i = 0; i < 9; i++)							// Loop for all PCBs
		{
			PCB_ptr = &PCBArray[i];							// Obtain pointer to PCB 
		
			PID = PCB_ptr->getPID();						// Convert integer values to strings
			snprintf (temp_PID, 8, "%d", PID);				
			State = PCB_ptr->getState();					
			snprintf (temp_State, 8, "%d", State);				
			Priority = PCB_ptr->getPriority();				
			snprintf (temp_Priority, 8, "%d", Priority);

			strcat(message,temp_PID);						// Concatenate temp strings to message string 
			strcat(message,temp1);
			strcat(message,temp_State);
			strcat(message,temp1);
			strcat(message,temp_Priority);			
			strcat(message,temp2);							// Concatenate process separator to message

		}
		
		toFill->setMessage (message);						// Copy message string into message envelope
		toFill->setMessageType (3);								// Set message envelope type to "console output"
				
		atomic(false);
		return 0;											// Return success
	}
	
	atomic(false);
	return 1;												// Invalid message envelope was provided, return error
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int RTX::change_priority (int new_priority, int target_PID)
{
	atomic(true);
	
	PCB *target_PCB;										// Pointer to target PCB
	PCBQ *PCBQ_ptr;
	int target_State;											// Pointer to queue target PCB resides on
															
	target_PCB = &PCBArray[target_PID];					// Obtain target PCB by searching it's PID
	target_State = target_PCB->getState();			// Obtain target PCB's state
	
	if (target_State == 0) {			
		target_PCB->setPriority (new_priority);	
	}
		
	else if (target_State == 1) {
		PCBQ_ptr = &RPQArray[target_PCB->getPriority()];	// Find which priority RPQ the target PCB is
		PCBQ_ptr->popTarget(target_PCB);				// Remove target PCB from that queue
		target_PCB->setPriority(new_priority);			// Update target PCB's priority
		PCBQ_ptr = &RPQArray[new_priority];				// Find which priority RPQ the target PCB should be placed on
		PCBQ_ptr->addNodeAtEnd(target_PCB);				// Enqueue onto appropriate RPQ
	}
	
	else if (target_State == 2) {
		PCBQ_ptr = &BORArray[target_PCB->getPriority()];	// Find which priority BOR queue the target PCB is
		PCBQ_ptr->popTarget(target_PCB);				// Remove target PCB from that queue
		target_PCB->setPriority(new_priority);			// Update target PCB's priority
		PCBQ_ptr = &BORArray[new_priority];				// Find which priority BOR queue the target PCB should be placed on
		PCBQ_ptr->addNodeAtEnd(target_PCB);				// Enqueue onto appropriate BOR queue
	}

	else if (target_State == 3) {
		PCBQ_ptr = &BOEArray[target_PCB->getPriority()];	// Find which priority BOE queue the target PCB is
		PCBQ_ptr->popTarget(target_PCB);				// Remove target PCB from that queue
		target_PCB->setPriority(new_priority);			// Update target PCB's priority
		PCBQ_ptr = &BOEArray[new_priority];				// Find which priority BOE queue the target PCB should be placed on
		PCBQ_ptr->addNodeAtEnd(target_PCB);				// Enqueue onto appropriate BOE queue
	}
		
	atomic(false);
	return 0;											// Return success
													// Return error code
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int RTX::get_trace_buffers (MsgEnv *toFill)
{
	atomic(true);
	
	char send_msg[512];
	char receive_msg[512];
	char temp[2] = {'\n','\0'};
	char header[32];
	
	if (toFill)
	{		
		send_msg[0] = receive_msg[0] = '\0';
		
		strcpy(header,"\n\nID\tsrcPid\tdestPid\ttime\tmsgType\n");
		strcat(send_msg,header);
			
		SendQ->sendTrace (send_msg);
		ReceiveQ->receiveTrace (receive_msg);
		
		strcat(send_msg,temp);
		strcat(send_msg,header);
		strcat(send_msg,receive_msg);
	
		toFill->setMessage(send_msg);						
		toFill->setMessageType(3);							

		atomic(false);
		return 0;											
	}
		
	atomic(false);
	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int RTX::send_console_chars(MsgEnv *env) {

	atomic(true);

	char *temp1;
	int pid, i;
	
	pid = current_process->getPID();
	
	if (env) {

		if (!shm2->flag) {
		
			temp1 = env->getMessage();
		
			shm2->data.setMessage(temp1);
			shm2->data.setMessageType(env->getMessageType());
			shm2->flag = true;
		
			if (env->getPIDFrom() == 3)
				env->clear();
			else {
				env->setMessageType(display_ack);
				rtx.send_message(env->getPIDFrom(),env);
			}
			atomic(false);
			return 1;
		}
		
		else
			PCBArray[2].getMessageEnvQueue()->addNodeAtEnd(env);
	}
	
	atomic(false);		
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int RTX::get_console_chars(MsgEnv *env) {

	atomic(true);

	char *temp1, *temp2;
	int i;
	
	temp2 = new char[512];
	
	if (env) {
		temp1 = shm1->data.getMessage();
		
		i = 0;
		while (temp1[i] != '\0') {
			temp2[i] = temp1[i];
			i++;
		}
		temp2[i] = '\0';	
		
		env->setMessage(temp2);
		rtx.send_message(env->getPIDTo(),env);
				
		atomic(false);
		return 0;
	}
		
	atomic(false);
	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int RTX::request_delay (int delay, int code, MsgEnv *env)
{
	atomic(true);
	
	double d = (double) delay*res/1000000;
	
	char delay_string[32];
	
	if (env) {		
		snprintf(delay_string, 32, "%lf", d);				
		env->setMessage(delay_string);
		env->setMessageType(code);
		
		rtx.send_message(3,env);
				
		atomic(false);
		return 0;
	}
		
	atomic(false);
	return 1;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void RTX::initialize (void)
{	
	int i, j, p;
	
	timeglobal.sec = 0;
	timeglobal.min = 0;
	timeglobal.hr = 0;
	
	for (i = 0; i < MAX_ENV; i++)
	{
		MsgEnv_toAdd[i].clear();
		MsgEnv_toAdd[i].setMsgID(i+1);
		FreeQ->addNodeAtEnd(&MsgEnv_toAdd[i]);
	}
	
	MsgEnv_iProc[0].clear();
	MsgEnv_iProc[0].setMessageType(iProc);
	MsgEnv_iProc[0].setMsgID(i+1);
	MsgEnv_iProc[1].clear();
	MsgEnv_iProc[1].setMessageType(iProc);
	MsgEnv_iProc[1].setMsgID(i+2);
	
	for (i = 0; i < 16; i++)
	{
		Trace_toAdd[i].clear();
		SendQ->addNodeAtEnd(&Trace_toAdd[i]);
	}
	
	for (j = i; j < 32; j++)
	{
		Trace_toAdd[j].clear();
		ReceiveQ->addNodeAtEnd(&Trace_toAdd[j]);
	}
	
	for (i = 0; i < 9; i++)
	{
		if (i==0) {
		
			PCBArray[i].setState(1);
			PCBArray[i].setPID(0);
			PCBArray[i].setPriority(3);
			PCBArray[i].setAtomicCount(0);
		}
		
		else if (i==1) {
			PCBArray[i].setState(0);
			PCBArray[i].setPID(1);
			PCBArray[i].setPriority(0);
			PCBArray[i].setAtomicCount(0);
		}
		
		else if (i==2) {
			PCBArray[i].setState(0);
			PCBArray[i].setPID(2);
			PCBArray[i].setPriority(0);
			PCBArray[i].setAtomicCount(0);
		}
		
		else if (i==3) {
			PCBArray[i].setState(0);
			PCBArray[i].setPID(3);
			PCBArray[i].setPriority(0);
			PCBArray[i].setAtomicCount(0);
		}
		
		else if (i==4) {
		
			PCBArray[i].setState(1);
			PCBArray[i].setPID(4);
			PCBArray[i].setPriority(0);
			PCBArray[i].setAtomicCount(0);
	
			p = PCBArray[i].getPriority();
			RPQArray[p].addNodeAtEnd(&PCBArray[i]);
		}
		
		else if (i==5) {
		
			PCBArray[i].setState(1);
			PCBArray[i].setPID(5);
			PCBArray[i].setPriority(2);
			PCBArray[i].setAtomicCount(0);
	
			p = PCBArray[i].getPriority();
			RPQArray[p].addNodeAtEnd(&PCBArray[i]);
		}
		
		else if (i==6) {
		
			PCBArray[i].setState(1);
			PCBArray[i].setPID(6);
			PCBArray[i].setPriority(3);
			PCBArray[i].setAtomicCount(0);
	
			p = PCBArray[i].getPriority();
			RPQArray[p].addNodeAtEnd(&PCBArray[i]);
		}
		
		else if (i==7) {
		
			PCBArray[i].setState(1);
			PCBArray[i].setPID(7);
			PCBArray[i].setPriority(1);
			PCBArray[i].setAtomicCount(0);
	
			p = PCBArray[i].getPriority();
			RPQArray[p].addNodeAtEnd(&PCBArray[i]);
		}
		
		else if (i==8) {
		
			PCBArray[i].setState(1);
			PCBArray[i].setPID(8);
			PCBArray[i].setPriority(2);
			PCBArray[i].setAtomicCount(0);
	
			p = PCBArray[i].getPriority();
			RPQArray[p].addNodeAtEnd(&PCBArray[i]);
		}
	}
	
	//shared memory for rx
	
	key1 = 12345000;

    if ((shmid1 = shmget(key1, sizeof(smStruct), IPC_CREAT | 0644)) < 0) { //644?
        
		perror("shmget");
        exit(1);
    }

    if ((shm1 = (smStruct*) shmat(shmid1, NULL, 0)) == (smStruct *) -1) {
       
		perror("shmat");
        exit(1);
    }
    
    shm1->flag = false;
   
   //shared memory for tx
    
    key2 = 12345600;

    if ((shmid2 = shmget(key2, sizeof(smStruct), IPC_CREAT | 0644)) < 0) { //644?
        
		perror("shmget");
        exit(1);
    }

    if ((shm2 = (smStruct*) shmat(shmid2, NULL, 0)) == (smStruct *) -1) {
        
		perror("shmat");
        exit(1);
    }
    
    shm2->flag = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void RTX::terminate (void)
{
	cout << "\n\nTerminating RTX ...\n\n";
	
	kill(rxpid,SIGINT);
	
	cout << rxpid << " (RX) killed\n";
	
	kill(txpid,SIGINT);
	
	cout << txpid << " (TX) killed\n";
	
	if (shmctl(shmid1, IPC_RMID, 0) < 0)
		cout << "Failed to delete RX shared memory\n";
		
	if (shmctl(shmid2, IPC_RMID, 0) < 0)
		cout<<"Failed to delete TX shared memory\n";
	
	free(PCBArray[0].stack);
	free(PCBArray[4].stack);
	free(PCBArray[5].stack);
	free(PCBArray[6].stack);
	free(PCBArray[7].stack);
	free(PCBArray[8].stack);
	
	exit(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
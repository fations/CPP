#include <iostream.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "RTX.h"

////////////////////////////////////////////////////////////////////////////////

void bsdsignal(int signo, void(*alarm_catcher)(int))
{
	struct sigaction act;
	act.sa_handler = alarm_catcher;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	if(sigaction(signo, &act,NULL) == -1) {
		perror("signal:");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void sigusr1(int) {  

	bsdsignal(SIGUSR1,sigusr1);
	
	MsgEnv *env;
	PCB *pcbPtr = current_process;

	current_process = &PCBArray[1];
	
	env = &MsgEnv_iProc[0];

	if (env) {
		env->setPIDFrom(1);
		env->setPIDTo(4);
		env->setMessageType(console_input);
		rtx.get_console_chars(env);
	}
	
	current_process = pcbPtr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void sigusr2(int) {  

    bsdsignal(SIGUSR2,sigusr2);
    
    MsgEnv *env;
	PCB *pcbPtr = current_process;
	
	current_process = &PCBArray[2];
	
	env = rtx.receive_message();
	
	if (env) 
		rtx.send_console_chars(env);
	
	shm1->data.clear();
	shm1->flag = false;
	
	current_process = pcbPtr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void sigalrm(int) 
{     
	bsdsignal(SIGALRM,sigalrm);

	PCB * pcbPtr;
	MsgEnv* env;
	int delay_int;
	int size;
	
	ticks++;
	
	pcbPtr = current_process;
	current_process = &PCBArray[3];
		
	if (TimeQ->hasNodes()) {
	
		TimeQ->updateDelay(res);
		size = TimeQ->getSize();
		
		for (int i = 0; i < size; i++) {
			env = TimeQ->popQueueNode();
			
			delay_int = atoi(env->getMessage());
			
			if (delay_int <= 0)
				rtx.send_message(env->getPIDFrom(), env);
			else
				TimeQ->addNodeAtEnd(env);
		}
	}
	
	env = rtx.receive_message();
	
	while (env){
		TimeQ->addNodeAtEnd(env);
		env = rtx.receive_message();
	}
	
	int tomod = 1000000/res; 
	
	if (ticks % tomod == 0) {
	
		env = &MsgEnv_iProc[1];
	
		if (env) {
			env->setPIDFrom(3);
			env->setPIDTo(2);
			env->setMessageType(update_time); 
			env->setMessage(format_time()); 
			rtx.send_console_chars(env);
		}
	}
	
	current_process = pcbPtr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void sigint(int) {  

	rtx.terminate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void processA(void)
{
	MsgEnv *msgEnvPtr;
	int num;
	char numchar[8];

	msgEnvPtr = NULL;

	while (!msgEnvPtr)
		msgEnvPtr = rtx.receive_message();
	
	rtx.release_msg_env(msgEnvPtr);
	
	num = 0;

	while (true)
	{	
		msgEnvPtr = NULL;
			
		while (!msgEnvPtr)
			msgEnvPtr = rtx.request_msg_env();
		
		memset(numchar,'\0',8);
		
		snprintf(numchar, 8,"%d", num);
		msgEnvPtr->setMessage(numchar);
		
		msgEnvPtr->setMessageType(count_report);
		
		rtx.send_message(7, msgEnvPtr);
		
		num++;
		
		rtx.release_processor();
	}
}

////////////////////////////////////////////////////////////////////////////////

void processB(void)
{
	MsgEnv *msgEnvPtr;
	int mType;

	while (true) {	
	
		msgEnvPtr = NULL;
	
		while (!msgEnvPtr)
			msgEnvPtr = rtx.receive_message();		
		
		rtx.send_message(8, msgEnvPtr);
	}
}

////////////////////////////////////////////////////////////////////////////////

void processC(void)
{
	MsgEnv *msgEnvPtr;
	char *data = new char[512];
	char *temp1, *temp2;
    int datanum, msgtype, i, i_count;
	
    char c_count[8];
	char *toSend = new char [512];
	
    temp2 = new char [512];

	while (true)
	{	
		msgEnvPtr = NULL;
		
		while (!msgEnvPtr)
			msgEnvPtr = rtx.receive_message();

		if (msgEnvPtr->getMessageType() == count_report) {

			temp1 = msgEnvPtr->getMessage();
			
			strcpy(temp2,temp1);
			
			datanum = atoi(temp2);
										
			if ((datanum % 20) == 0) {

				i_count = datanum / 20;
				memset(c_count,'\0',8);
				snprintf (c_count, 8, "%d", i_count);				
				strcpy(toSend,"\n\nPROCESS C COUNT: \0");
				strcat(toSend,c_count);
				
				msgEnvPtr->setMessage(toSend);
								
				msgEnvPtr->setPIDFrom(8);
				msgEnvPtr->setPIDTo(2);
				msgEnvPtr->setMessageType(console_output);
				rtx.send_console_chars(msgEnvPtr);

				do {		
					msgEnvPtr = NULL;
		
					while (!msgEnvPtr)
						msgEnvPtr = rtx.receive_message();

					if (msgEnvPtr->getMessageType() != display_ack) {
						rtx.release_msg_env(msgEnvPtr);
//						rtx.send_message(8, msgEnvPtr);
					}
						
				} while (msgEnvPtr->getMessageType() != display_ack);

				int todelay = 10000000/res;

				rtx.request_delay(todelay, 4, msgEnvPtr);

				do {		
					msgEnvPtr = NULL;
		
					while (!msgEnvPtr)
						msgEnvPtr = rtx.receive_message();

					if (msgEnvPtr->getMessageType() != wakeup) {
//						rtx.release_msg_env(msgEnvPtr);
						rtx.send_message(7, msgEnvPtr);
					}
						
				} while (msgEnvPtr->getMessageType() != wakeup);
			}
		}

		rtx.release_msg_env(msgEnvPtr);
		rtx.release_processor();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void cci(void) {
	
	MsgEnv *env;
	char *temp1, *temp2;
	int i, newpriority, pid;
	int stringlength;
	bool start = false;
	
	temp2 = new char [512];
		
	while (true) {
				
		do
			env = rtx.receive_message();
		while (!env);
		
				
		if (env->getMessageType() == console_input) {

			temp1 = env->getMessage();
			
			memset(temp2,'\0',512);
			strcpy(temp2,temp1);
			stringlength = strlen(temp2);
			
			env->clear();
				
			if ((temp2[0] == 'p' && stringlength == 1) || (temp2[0] == 'P' && stringlength == 1)) {
					
				do
					env = rtx.request_msg_env();
				while (!env);
					
				rtx.request_process_status(env);
					
				env->setPIDFrom(4);
				env->setPIDTo(2);
				env->setMessageType(console_output);
				
				rtx.send_console_chars(env);
			}
				
			else if ((temp2[0] == 'n' && stringlength == 3)  || (temp2[0] == 'N' && stringlength == 3)) {
				
				char new_p_char = temp2[1];
				int new_p_int = atoi(&new_p_char);
			
				if ((new_p_int >= 0) && (new_p_int <= 3)) {
												
					char pid_char = temp2[2];
					int pid_int = atoi(&pid_char);
							
					if ((pid_int > 0) && (pid_int <= 8) && (pid_int != 4)) {
						
						rtx.change_priority(new_p_int,pid_int);
								
						do
							env = rtx.request_msg_env();
						while (!env);
					
						env->setMessage("\n\nPriority Change Successful\n");
						env->setPIDFrom(4);
						env->setPIDTo(2);
						env->setMessageType(console_output);
					
						rtx.send_console_chars(env);
					}
							
					else {
						do
							env = rtx.request_msg_env();
						while (!env);
					
						env->setMessage("\n\nInvalid PID\n");
						env->setPIDFrom(4);
						env->setPIDTo(2);
						env->setMessageType(console_output);
					
						rtx.send_console_chars(env);
					}
				}
						
				else {
					do
						env = rtx.request_msg_env();
					while (!env);
					
					env->setMessage("\n\nInvalid Priority\n");
					env->setPIDFrom(4);
					env->setPIDTo(2);
					env->setMessageType(console_output);
					
					rtx.send_console_chars(env);
				}
			}
				
			else if ((temp2[0] == 'b' && stringlength == 1) || (temp2[0] == 'B' && stringlength == 1)) {
					
				do
					env = rtx.request_msg_env();
				while (!env);
					
				rtx.get_trace_buffers(env);

				env->setPIDFrom(4);
				env->setPIDTo(2);
				env->setMessageType(console_output);
					
				rtx.send_console_chars(env);
			}
				
			else if ((temp2[0] == 'c' && stringlength == 9) || (temp2[0] == 'C' && stringlength == 9)) {
			
				if ((temp2[3] == ':') && (temp2[6] == ':'))
				{
					char *c_hour = new char [2];
					char *c_min = new char [2];
					char *c_sec = new char [2];
					
					int i_hour;
					int i_min;
					int i_sec;
						
					c_hour[0] = temp2[1];
					c_hour[1] = temp2[2];
					
					c_min[0] = temp2[4];
					c_min[1] = temp2[5];
					
					c_sec[0] = temp2[7];
					c_sec[1] = temp2[8];
						
					i_hour = atoi(c_hour);
					i_min = atoi(c_min);
					i_sec = atoi(c_sec);
						
					if ((i_hour >=0) && (i_hour <=23)) {
					
						if ((i_min >=0) && (i_min <=59)) {
						
							if ((i_sec >=0) && (i_sec <=59)) {
								
								timeglobal.sec = i_sec;
								timeglobal.min = i_min;
								timeglobal.hr = i_hour;
									
								do
									env = rtx.request_msg_env();
								while (!env);
					
								env->setMessage("\n\nClock Update Successful\n\n");
								env->setPIDFrom(4);
								env->setPIDTo(2);
								env->setMessageType(console_output);
					
								rtx.send_console_chars(env);
							}
								
							else {
								do
									env = rtx.request_msg_env();
								while (!env);
					
								env->setMessage("\n\nInvalid Time\n");
								env->setPIDFrom(4);
								env->setPIDTo(2);
								env->setMessageType(console_output);
					
								rtx.send_console_chars(env);
							}
						}
							
						else {
							do
								env = rtx.request_msg_env();
							while (!env);
					
							env->setMessage("\n\nInvalid Time\n");
							env->setPIDFrom(4);
							env->setPIDTo(2);
							env->setMessageType(console_output);
					
							rtx.send_console_chars(env);
						}
					}
						
					else {
						do
							env = rtx.request_msg_env();
						while (!env);
				
						env->setMessage("\n\nInvalid Time\n");
						env->setPIDFrom(4);
						env->setPIDTo(2);
						env->setMessageType(console_output);
					
						rtx.send_console_chars(env);
					}
				}
					
				else {
					do
						env = rtx.request_msg_env();
					while (!env);
				
					env->setMessage("\n\nInvalid Time Format\n");
					env->setPIDFrom(4);
					env->setPIDTo(2);
					env->setMessageType(console_output);
				
					rtx.send_console_chars(env);
				}
			}
			
			else if (((temp2[0] == 's' && stringlength == 1) || (temp2[0] == 'S' && stringlength == 1)) && (start == false)) {
				
				do
					env = rtx.request_msg_env();
				while (!env);
				
				if (start == false) {
					
					start = true;
					env->setPIDFrom(4);
					env->setPIDTo(6);
					env->setMessageType(console_input);
				
					rtx.send_message(6, env);
				}
				
			}
			
			else if (((temp2[0] == 's' && stringlength == 1) || (temp2[0] == 'S' && stringlength == 1)) && (start == true)) {
				
				do
					env = rtx.request_msg_env();
				while (!env);
					
				env->setMessage("\n\nDemonstration Already Begun\n");
				env->setPIDFrom(4);
				env->setPIDTo(2);
				env->setMessageType(console_output);
					
				rtx.send_console_chars(env);
			}
			
			else if ((temp2[0] == 't' && stringlength == 1) || (temp2[0] == 'T' && stringlength == 1)) {
					
				rtx.terminate();
			}
				
			else {
				do
					env = rtx.request_msg_env();
				while (!env);
					
				env->setMessage("\n\nInvalid Input\n");
				env->setPIDFrom(4);
				env->setPIDTo(2);
				env->setMessageType(console_output);
					
				rtx.send_console_chars(env);
			}
		}
		
		else 
			rtx.release_msg_env(env);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void null_process(void) {

	while (true)
		rtx.release_processor();	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void clock_process(void)
{	
	MsgEnv *env;
	
	while (true) {
	
		do {
			env = rtx.receive_message();
		
			if (env) {
			
				if (env->getMessageType() == display_ack) {
					rtx.release_msg_env(env);
					
					env = NULL;
				}
				else
					rtx.release_msg_env(env);
			}
		} while (!env);
	
		do
			env = rtx.request_msg_env();
		while (!env);
	
		int todelay = 5*1000000/res;
	
		rtx.request_delay(todelay, wakeup, env);
		
		do
			env = rtx.receive_message();			
		while (!env);

		env->setMessage("\n\n5 SECONDS PASSED\n");
		env->setPIDFrom(5);
		env->setPIDTo(2);
		env->setMessageType(console_output);
					
		rtx.send_console_chars(env);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    PCBArray[0].start = null_process;
    PCBArray[4].start = cci;
    PCBArray[5].start = clock_process;
    PCBArray[6].start = processA;
    PCBArray[7].start = processB;
    PCBArray[8].start = processC;
    
    initialize_PCB(PCBArray[0]);
    initialize_PCB(PCBArray[4]);
    initialize_PCB(PCBArray[5]);
    initialize_PCB(PCBArray[6]);
    initialize_PCB(PCBArray[7]);
    initialize_PCB(PCBArray[8]);
    
    rtx.initialize();
 
    if ((txpid = fork()) < 0) {
	
		cout<<"\nCouldn't fork!";
		return 0;
	}
	
	if (txpid == 0) {
	
		execl("tx", 0);
		cout<<"\nError1";
	}
	
	else {	
	
		sleep(1);
	
		if ((rxpid = fork()) < 0) {
		
			cout<<"\nCouldn't fork!";
			return 0;
		}
	
		if (rxpid == 0) {
		
			execl("rx", 0);
			cout<<"\nError2";
		} 
		else {
		
		 	sleep(1);

			bsdsignal(SIGUSR1,sigusr1);
			bsdsignal(SIGUSR2,sigusr2);
			bsdsignal(SIGINT,sigint);
			bsdsignal(SIGALRM,sigalrm);
	
		 	ualarm(res,res);
		 	
		 	current_process = &PCBArray[0];
		 	null_process();
		}

	}
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "MsgEnv.h"
#include "shm.h"

int main() {

	char save_loc[3] = {27, '7', '\0'};
	char rest_loc[3] = {27, '8', '\0'};
	char erase_scr[5] = {27, '[', '2', 'J', '\0'};
	char erase_below[4] = {27, '[', 'J', '\0'};
	char erase_line[4] = {27, '[', 'K', '\0'};
	char corner[8] = {27, '[', '1', ';', '7', '2', 'f', '\0'};
	char home[7] = {27, '[', '3', ';', '1', 'f', '\0'};
	
	char prompt[7] = {'c', 'c', 'i', '>', '>', ' ', '\0'};

    int shmid, ppid;
    key_t key;
    smStruct *shm, *s;
    char *temp;
    int msgType;
   
    ppid = getppid();
   
    key = 12345600;

    if ((shmid = shmget(key, sizeof(smStruct), 0644)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm = (smStruct*) shmat(shmid, NULL, 0)) == (smStruct *) -1) {
        perror("shmat");
        exit(1);
    }
   
    s = shm;
    
    fputs(erase_scr, stdout);
	fputs(home, stdout);
	fputs(prompt, stdout);
	
    for(;;) {
		
		if (s->flag == true) {
		
			temp = s->data.getMessage();
			msgType = s->data.getMessageType();
			
			if (msgType == 6) {
				fputs(save_loc, stdout);
				fputs(corner, stdout);
				fputs(temp, stdout);
				fputs(rest_loc, stdout);	
			}
			
			else {
				fputs(erase_below, stdout);
				fputs(temp, stdout);
				fputs(home, stdout);
				fputs(prompt, stdout);
				fputs(erase_line, stdout);
			}
			
			s->data.clear();
			s->flag = false;
			
			kill(ppid,SIGUSR2);		
		}
		else
			usleep(50);
    }
    return 0;
}
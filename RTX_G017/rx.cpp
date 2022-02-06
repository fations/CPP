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
#include "MsgEnv.h"
#include "shm.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    int shmid,ppid;
    size_t size;
    key_t key;
    smStruct *shm, *s;
    
	char temp[512];
	char temp2[512];
	int i, j, check;
    
    ppid = getppid();
    
    key = 12345000;

    if ((shmid = shmget(key, sizeof(smStruct), 0644)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm = (smStruct*) shmat(shmid, NULL, 0)) == (smStruct *) -1) {
        perror("shmat");
        exit(1);
    }

	s = shm;

    while (true) {
		if (s->flag == false) {

			memset(temp2,'\0',512);
			memset(temp,'\0',512);
			
			cin.getline(temp,512);

			j = 0;
			
			for (i = 0; i < 512; i++) {
			
				check = (int) temp[i];
				
				if (((check >= 97) && (check <= 122)) || ((check >= 65) && (check <= 90)) || ((check >= 48) && (check <= 58))) {

					temp2[j] = temp[i];
					j++;
				}
			}
			
			temp2[j+1] = '\0';
			
			s->data.setMessage(temp2);		
			s->flag = true;
			
			kill(ppid, SIGUSR1);
		}
		else
			usleep(50);
	}
	return 0;
}
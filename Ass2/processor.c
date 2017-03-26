#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(void) {

	int smID, count, index;
	char *shm, *s;
	char line[100];

	FILE *secretFile;
	secretFile = fopen("secrets.out","w");

	/***********Setup shared memory*************/

	key_t key = getuid();   // Get key equal to the user ID created by receiver.c

	smID = shmget(key, 1024, 0666);   // Locate shared memory segment
	if (smID < 0) {   		  // Error: Segment isn't located
		perror("shmget failed: shared memory segment wasn't created");
		exit(EXIT_FAILURE);
	}

	shm = shmat(smID, NULL, 0);   // Attach to shared memory
	if (shm == (char *) -1) {     // Error: shared memory segment isn't attached
		perror("shmat failed: segment wasn't attached");
		exit(EXIT_FAILURE);
	}

	/***********Process User Input*************/

	while(1) {
		if (*shm != (int) NULL) {   // Wait for user input
			if (*shm != '@') {  // There is a new input.
				//initialize line
				index = 0, count = 0;
				memset(line, 0, sizeof(line)); 

				// Receive string, and counts the number of characters
				for (s = shm; *s != '#'; s++) {
					line[index++] = *s;
					if(isdigit(*s)) {
						count++;
					}
				}
				
				*shm = '@'; // Mark end symbol of reading.
				
				// Write to 'secrets.out' file.
				secretFile = fopen ("secrets.out","a");
				if (secretFile != NULL) {
					fprintf(secretFile, "%d: ", count);
					fputs(line, secretFile);
					fclose(secretFile);
				}
			}
		}
	}

	/********* Dettach **********/

	if (shmdt(shm) == -1) { // Error: Shared memory isn't dettached
		perror("shmdt failed: segment wasn't dettached");
		exit(1);
	}
}

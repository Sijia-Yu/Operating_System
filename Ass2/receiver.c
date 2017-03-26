#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(void) {
	
	int smID;
	char *shm, *s;
	char *secret = "C00L";
	char *userInput = malloc(sizeof(char)*100);	
	key_t key = getuid();  // Set key of the shared memory segment equal to the user ID

	smID = shmget(key, 1024, IPC_CREAT | 0666);   // Create shared memory segment
	if (smID < 0) {   			      // Error: Segment isn't created
		perror("shmget failed: shared memory segment wasn't created");
		exit(EXIT_FAILURE);
	}
	
	shm = shmat(smID, NULL, 0);   // Attach to shared memory
	if (shm == (char *) -1) {     // Error: shared memory segment isn't attached
		perror("shmat failed: segment wasn't attached");
		exit(EXIT_FAILURE);
	}

	while(1) {
		printf("User Input $> ");
		fgets(userInput, 100, stdin);
		
		if(strstr(userInput, secret) == NULL) { //Ignore the input doesn't contain "C00L".
			continue;	
		}

		// Copy input to shared memory
		memcpy(shm, userInput, strlen(userInput));

		// Add end symbol of the input string
		s = shm;
		s += strlen(userInput);
		*s = '#';

		// Sleep until meet the finish reading symbol '@' in Processor.c
		while (*shm != '@') {
			sleep(1);
		}
	}

	if (shmdt(shm) == -1) { // Error: Shared memory isn't dettached
		perror("shmdt failed: segment wasn't dettached");
		exit(1);
	}

	if (shmctl(smID, IPC_RMID, 0) == -1) { // Error: The shared memory IPC communication doesn't close
		perror("shmctl failed: ipc didn't close");
		exit(1);
	}
}

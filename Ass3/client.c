#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netdb.h>

#define PORT "9010"
#define SER_IP "192.168.1.158"

//error handling
void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(void) {
	int sock;
	struct sockaddr_in serv_addr;
	char *user_input = malloc(sizeof(char)*BUFSIZ);
	char *secret = "C00L";

	while(1) {
		printf("Please enter $>: ");
		fgets(user_input, BUFSIZ, stdin);

		//Input doesn't contain secret word, continue
		if(strstr(user_input, secret) == NULL) {
			continue;	
		}

		memset(&serv_addr, 0, sizeof(serv_addr));

		//Create socket
		sock = socket(PF_INET, SOCK_STREAM, 0);
		if (sock == -1) {
			error_handling("socket() error");
		}

		//Set address (IP and PORT)
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr(SER_IP); //server IP
		serv_addr.sin_port = htons(atoi(PORT));

		//Connect socket
		if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1) {
			error_handling("connect() error");
		}

		printf("Client connecting to %s\n",inet_ntoa(serv_addr.sin_addr));
		
		//Send input to server
		if (send(sock, user_input, strlen(user_input), 0) < 0) {
			error_handling("send() error");
		}
		close(sock);
	}
	return 0;
}

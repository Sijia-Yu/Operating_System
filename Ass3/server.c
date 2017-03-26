#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<signal.h>
#include<ctype.h>

#define PORT "9010"
int countLine = 0;
int countTotalDigits = 0;

//error handling
void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

//count digits in an input string
int countDigits(char *s) {
    int count = 0;
    while (*s != '\0') {
    	if (isdigit(*s)) {
       		count++;
        }
    	s++;
    }
    return count;
}

//call signal function when press Ctrl + C
void sig_callback(int signum) {
    switch (signum) {
        case SIGINT:
            //Fired when press Ctrl+C
            printf("Get signal SIGINT. \r\n");
            printf("Line = %d\n", countLine);
            printf("TotalDigits = %d\n", countTotalDigits);
            exit(0);  //exit after display
            break;
        default:
            //Other signals
            printf("Unknown signal %d. \r\n", signum);
            break;
    }
    return;
}

int main(void) {
	FILE *secret;
	secret = fopen("secret.out","w");
	fclose(secret);
	
	//Regist function for SIGINT
	signal(SIGINT, sig_callback);
	
	while(1) {
		int serv_sock;
		int clnt_sock;
		int buf_len;
		char buf[BUFSIZ];

		//Set address for server and client
		struct sockaddr_in serv_addr;
		struct sockaddr_in clnt_addr;
		socklen_t clnt_addr_size = sizeof(clnt_addr);
	
		memset(&serv_addr, 0, sizeof(serv_addr));

		//Create socket
		serv_sock = socket(PF_INET, SOCK_STREAM, 0);
		if (serv_sock == -1) {
			error_handling("socket() error");
		}

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(atoi(PORT));

		// Bind
		if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1) {
			error_handling("bind() error");
		}

		// Listen
		if (listen(serv_sock, 1) == -1) {
			error_handling("lisen() error");
		}

		// Accept
		clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
		if (clnt_sock == -1) {
			error_handling("accept() error");
		}
		
		printf("Server got a connection from %s\n",inet_ntoa(clnt_addr.sin_addr));

		//Receive input from client
		buf_len = recv(clnt_sock, buf, BUFSIZ-1, 0);
		if (buf_len < 0) {
			perror("receive nothing from client");
			exit(1);
		}
		buf[buf_len] = '\0';
		
		//Process input
		printf("Server received: %s", buf);
		secret = fopen("secret.out","a");
		fprintf(secret, "%d: ", countDigits(buf));
		fputs(buf, secret);
		fclose(secret);

		countLine++;
		countTotalDigits += countDigits(buf);
		
		//Close socket
		close(clnt_sock);
		close(serv_sock);
	}
	return 0;
}

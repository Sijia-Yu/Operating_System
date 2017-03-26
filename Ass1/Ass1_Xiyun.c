#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define SYMBOL " \t\r\n\a"
#define BUFSIZE 1024;

void shell_parse(char *read_in,char **tokenOutput) {
    int count = 0;
    char *token;
    token = strtok(read_in, SYMBOL);
    while(token != NULL) {
        tokenOutput[count] = token;
        count++;
        token = strtok(NULL, SYMBOL);
    }
    tokenOutput[count] = NULL;
}

void shell_execute(char **tokenOutput) { 
    pid_t pid;
    int status;
    pid = fork();
    if(pid == 0) { //child process
        if((execvp(tokenOutput[0], tokenOutput)) < 0){
            fprintf(stderr, "execution failure: %s\n", strerror(errno));
        }
    }else if(pid<0){
        fprintf(stderr, "fork failure: %s\n", strerror(errno));
    }else{
        while(wait(&status)!=pid);
    }
}

int main(int argc, char* argv[]){
    int buf_size = BUFSIZE;
    char *read_in = malloc(sizeof(char) * buf_size);
    size_t bufsize = BUFSIZE;
    char *tokenOutput[64];

    while(1){
        printf("shell->");

        getline(&read_in, &bufsize, stdin);
        shell_parse(read_in,tokenOutput);

        if((strcmp(tokenOutput[0],"exit")) == 0){
            exit(EXIT_SUCCESS);
        }
        if((strcmp(tokenOutput[0],"cd")) == 0){
            if(chdir(tokenOutput[1]) != 0){
               fprintf(stderr, "change dir failure: %s\n", strerror(errno));
            }
        }
        shell_execute(tokenOutput);
    }
    return 0;
}

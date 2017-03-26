#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MAXSIZE 80

//Generate substring
char* substring(char* inputChar,int pos,int length) {
    int i;
    char* pch = inputChar;
    char* subch = (char*)calloc(sizeof(char),length + 1); //Define a char*, point to ch
    
    pch = pch + pos; //pch point to pos
    
    for(i = 0; i < length; i++) {
        subch[i] = *(pch++);
    }
    subch[length]='\0';  //add '\0' to the end
    return subch;       //return the substring
}

//Skip space
char* skipSpace(char buffer[]) {
    char* result = buffer;
    
    //Use two pointers to determine the position of first and last character
    int startIndex = 0;
    while(startIndex < (int)strlen(buffer) && buffer[startIndex] == ' ') {
        startIndex++;
    }
    
    int endIndex = (int)strlen(buffer) - 1;
    while(endIndex >= 0 && (buffer[endIndex] == ' ' || buffer[endIndex] == '\n')) {
        endIndex--;
    }
    return substring(result, startIndex, (endIndex - startIndex + 1));
}

//Split the input command
void splitCommand(char** argv, char* s) {
    int i = 0, index = 0, pos = 0, len = 0;
    for(i = 0; i < strlen(s); i++) {
        if(s[i] == ' ') {
            argv[index++] = substring(s, pos, len);
            len = 1;
            pos = i + 1;
        }
        else {
            len++;
        }
    }
    argv[index] = substring(s, pos, len);
}

//Process shell command
void executeShell(char* argv[]) {
    int status;
    pid_t pid = fork();
    
    if(pid == -1) {
        printf("Fork error\n");
        exit(1);
    }
    else if(pid == 0) { //Child process
        if((strcmp(argv[0],"cd")) != 0) { //other command
            execvp(argv[0], argv);
            perror("Execution failed");
            exit(1);
        }
        else { //cd ..
            if(chdir(argv[1]) != 0) {
                fprintf(stderr, "change dir failure: %s\n", strerror(errno));
            }
        }
    }
    else { //Parent process
        pid = waitpid(pid, &status, 0);
    }
    printf("yusj$ >");
}

int main() {
    char buffer[MAXSIZE];
    
    printf("Please input your command:\n");
    printf("yusj$ >");
    
    //read commands
    while(fgets(buffer, MAXSIZE, stdin) != NULL) {
        
        char* s = skipSpace(buffer);
        char* argv[10] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
        splitCommand(argv, s); //split command by ' ', and store in char* argv[]
        
        //Execute shell command
        executeShell(argv);
    }
    return 0;
}

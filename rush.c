/*
Name: Jawaad Ramcharitar
Description: A simple shell program named rush, meant to fulfill the basic functions of a shell program:
    1. Implement basic shell using fork, exec, wait
    2. Utilize path to run external programs
    3. Include built-in commands for exit, cd, and path
    4. Allow for redirecion
    5. Print required error messages           
*/
#include "rush.h"

char** path = NULL; //global variable contianing path 
int path_count = 0; //global variable containing number of accepted paths

void print_emsg(){
    char error_message[30] = "An error has occurred\n"; //standard error message
    write(STDERR_FILENO, error_message, strlen(error_message)); //write to stderr
}

void set_path(char **args) {
    int i;

    //clear original path 
    for (i = 0; i < path_count; i++) {
        free(path[i]);
    }
    free(path);

    //count the number of args passed as new paths
    int count = 0;
    for (i = 1; args[i] != NULL; i++) {
        count++;
    }

    //allocate memory for new paths
    path = malloc(sizeof(char*) * count);
    if (path == NULL) {
        print_emsg();
        return;
    }

    //add args (new paths) to path variable
    for (i = 0; i < count; i++) {
        path[i] = strdup(args[i+1]); //allocate memory and copy
        //if failed free memory and return
        if (path[i] == NULL) { 
            print_emsg();
            int j;
            for (j = 0; j < i; j++) {
                free(path[j]);
            }
            free(path);
            path = NULL;
            path_count = 0;
            return;
        } 
    }
    path_count = count; //update path_count 
}


int main(int argc, char *argv[]){

    //check if shell invoked with argument(s)
    if(argc != 1){
        print_emsg();
        exit(1);
    }

    //allocate and set original path to include /bin
    path = malloc(sizeof(char*));
    if (path == NULL) {
        print_emsg();
        exit(1);
    }
    path[0] = strdup("/bin"); //allocate memory and copy
    path_count = 1;

    //declare buffer to handle input to the shell
    char* buffer;
    size_t buffsize = MAX_INPUT; //size set to max
    char* args[MAX_ARGS]; //declare args array to hold tokenized inputs to shell

    //allocate buffer
    buffer = (char*)malloc(buffsize * sizeof(char));
    if( buffer == NULL)
    {
        print_emsg();
        exit(1);
    }

    //enter while loop of shell 
    while(1){

        printf("rush> "); //print prompt
        fflush(stdout);

        //accept input to shell
        if (getline(&buffer, &buffsize, stdin) == -1) {
            continue; 
        }

        buffer[strcspn(buffer, "\n")] = '\0'; //remove terminating null character

        char *token; //used to help tokenize input
        int i = 0;
        //tokenize input from buffer
        while ((token = strsep(&buffer, " ")) != NULL) {
            if (*token == '\0') continue;
            args[i++] = token;
        }
        args[i] = NULL;

        //if input is empty continue
        if(args[0] == NULL){
            continue;
        }
        //execute built-in exit command
        else if(strcmp(args[0], "exit") == 0){
            //ensure no arguments passed
            if(args[1] != NULL){
                print_emsg();
                continue;
            }
            else{
                //free memory and exit
                free(buffer);
                for (i = 0; i < path_count; i++) {
                    free(path[i]);
                }
                free(path);
                exit(0);
            }
        }
        //execute built-in cd command
        else if(strcmp(args[0], "cd") == 0){
            //ensure correct format of command
            if(args[1] == NULL || args[2] != NULL){
                print_emsg();
                continue;
            }
            else{
                //change direcotry
                if(chdir(args[1]) == -1){
                    print_emsg();
                    continue;
                }
                else
                    continue;
            }
        }
        //execute built-in path command
        else if(strcmp(args[0], "path") == 0){
            set_path(args); //call set_path to change path
        }
        //execute external commands
        else{

            int rc = fork(); //fork process to run external command in child process
            //if fork fail
            if(rc < 0){
                print_emsg();
                continue;
            }
            //if fork successful
            else if (rc == 0) {

                char* outfile = NULL; //used to store new output location in case of redirection
                int index = -1;
                int count = 0;

                //check for redirection
                for (i = 1; args[i] != NULL; i++) {
                    if (strcmp(args[i], ">") == 0) {
                        count++;
                        index = i;

                        //ensure ">" is not used more than once
                        if (count > 1) {
                            print_emsg();
                            exit(1);
                        }

                        //ensure redirection is specified correctly
                        if (args[i+1] == NULL || args[i+2] != NULL) {
                            print_emsg();
                            exit(1);
                        }

                        outfile = args[i+1]; //store name of new output
                    }
                }

                //truncate args to execute redirection correctly 
                if (index != -1) {
                    args[index] = NULL; 
                }

                //open new output file
                if (outfile != NULL) {
                    close(STDOUT_FILENO);
                    int fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
                    if (fd < 0) {
                        print_emsg();
                        exit(1);
                    }
                }

                //search paths for executable 
                for (i = 0; i < path_count; i++) {
                    char full_path[256];

                    //specify path and command to check for executable
                    strcpy(full_path, path[i]);   
                    strcat(full_path, "/");       
                    strcat(full_path, args[0]);  

                    //check if executable exists in paths
                    if (access(full_path, X_OK) == 0) {  
                        args[0] = full_path;
                        execv(full_path, args);   //execute external command       
                        print_emsg();    //should not run if exec successful                
                        exit(1);
                    }
                }
                print_emsg(); //executable not found in paths
                exit(1);
            }
            else{
                wait(NULL); //parent waits on child 
            }

        }

    }

    return 0;
}



/*
Name: Jawaad Ramcharitar
UID: 10335787
NetID: jawaadramcharitar
Description: A simple shell program named rush, meant to fulfill the basic functions of a shell program:
    1. Implement basic shell using fork, exec, wait
    2. Utilize path to run external programs
    3. Include built-in commands for exit, cd, and path
    4. Allow for redirecion
    5. Print required error messages           
*/
#ifndef RUSH_H
#define RUSH_H

//Include all required libraries 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_INPUT 256 //Maximum acceptable number of char as input to shell
#define MAX_ARGS 256 //Maximum number of args to shell 

/*************************************************************
	Function Name - print_emsg()
	@brief Print the standard error message to stderr
	@param None
*************************************************************/
void print_emsg(); 

/*************************************************************
	Function Name - set_path(char **args)
	@brief Updates the path variable with user inputted args 
	@param Pointer to array of char pointers containing the args passed by user
*************************************************************/
void set_path(char **args);

#endif

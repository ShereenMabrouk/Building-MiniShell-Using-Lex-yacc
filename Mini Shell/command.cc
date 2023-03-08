 /* Shereen Mostafa Hassan Mabrouk   6844
    Hend Khaled Abdelhamid Mohamed Aly 6986
*/

/*
 * CS354: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include<iostream>
#include<string>
#include<time.h>
#include <ctime>
#include "command.h"
SimpleCommand::SimpleCommand()
{
	// Creat available space for 5 arguments
	_numberOfAvailableArguments = 5;
	_numberOfArguments = 0;
	_arguments = (char **)malloc(_numberOfAvailableArguments * sizeof(char *));
}

void SimpleCommand::insertArgument(char *argument)
{
	if (_numberOfAvailableArguments == _numberOfArguments + 1)
	{
		// Double the available space
		_numberOfAvailableArguments *= 2;
		_arguments = (char **)realloc(_arguments,
									  _numberOfAvailableArguments * sizeof(char *));
	}

	_arguments[_numberOfArguments] = argument;

	// Add NULL argument at the end
	_arguments[_numberOfArguments + 1] = NULL;

	_numberOfArguments++;
}

Command::Command()
{
	// Create available space for one simple command
	_numberOfAvailableSimpleCommands = 1;
	_simpleCommands = (SimpleCommand **)
		malloc(_numberOfSimpleCommands * sizeof(SimpleCommand *));

	_numberOfSimpleCommands = 0;
	_outFile = 0;
	_inputFile = 0;
	_errFile = 0;
	_background = 0;
}

void Command::insertSimpleCommand(SimpleCommand *simpleCommand)
{
	if (_numberOfAvailableSimpleCommands == _numberOfSimpleCommands)
	{
		_numberOfAvailableSimpleCommands *= 2;
		_simpleCommands = (SimpleCommand **)realloc(_simpleCommands,
													_numberOfAvailableSimpleCommands * sizeof(SimpleCommand *));
	}

	_simpleCommands[_numberOfSimpleCommands] = simpleCommand;
	_numberOfSimpleCommands++;
}

void Command::clear()
{
	for (int i = 0; i < _numberOfSimpleCommands; i++)
	{
		for (int j = 0; j < _simpleCommands[i]->_numberOfArguments; j++)
		{
			free(_simpleCommands[i]->_arguments[j]);
		}

		free(_simpleCommands[i]->_arguments);
		free(_simpleCommands[i]);
	}

	if (_outFile)
	{
		free(_outFile);
	}

	if (_inputFile)
	{
		free(_inputFile);
	}

	if (_errFile)
	{
		free(_errFile);
	}

	_numberOfSimpleCommands = 0;
	_outFile = 0;
	_inputFile = 0;
	_errFile = 0;
	_background = 0;
}

void Command::print()
{
	printf("\n\n");
	printf("              COMMAND TABLE                \n");
	printf("\n");
	printf("  #   Simple Commands\n");
	printf("  --- ----------------------------------------------------------\n");

	for (int i = 0; i < _numberOfSimpleCommands; i++)
	{
		printf("  %-3d ", i);
		for (int j = 0; j < _simpleCommands[i]->_numberOfArguments; j++)
		{
			printf("\"%s\" \t", _simpleCommands[i]->_arguments[j]);
		}
	}

	printf("\n\n");
	printf("  Output       Input        Error        Background\n");
	printf("  ------------ ------------ ------------ ------------\n");
	printf("  %-12s %-12s %-12s %-12s\n", _outFile ? _outFile : "default",
		   _inputFile ? _inputFile : "default", _errFile ? _errFile : "default",
		   _background ? "YES" : "NO");
	printf("\n\n");
}
/////////////////Handling Ctrl c and log ///////////////////////////////////
void ctrlcfun(int){
	
	
		printf("\n");
		Command::_currentCommand.prompt();	
	
}

void logchild(int){
	FILE *f;
	//int pid;
	//pid=wait(NULL);
		std::time_t result = std::time(nullptr);
		f=fopen("child.log","a");
		//fprintf(f,"A child is create with pid %d \n",pid);
		fprintf(f,"Child is terminated\n");
		fprintf(f,std::ctime(&result));
		fclose(f);
}
///////////////////////////End of functions ////////////////////////////////////////
void Command::execute()
{
	// Don't do anything if there are no simple commands
	if (_numberOfSimpleCommands == 0)
	{
		prompt();
		return;
	}
	

	// Print contents of Command data structure
	print();

	// Add execution here
	
    /////////////////////////  Our Execution Code Start Here ////////////////////////////////////
	
	//Define Variables 
	
	int pid;
	int defaultin = dup(0);	 
	int defaultout = dup(1); 
	int defaulterr = dup(2); 

	int newfileinput = 0;
	int newfileoutput = 0;
	int newfileerror = 0;



	for (int i = 0; i < _numberOfSimpleCommands; i++)
	{
		
		if(!strcmp(_simpleCommands[i]->_arguments[0],"exit")){
			_exit(0);
		}
		if(!strcmp( _simpleCommands[0]->_arguments[0], "cd" )){
			
	    	int ret;
	   		 if(_simpleCommands[0]->_numberOfArguments == 1)
	      	ret = chdir(getenv("HOME"));
	    	else
	      		ret = chdir(_simpleCommands[0]->_arguments[1]);
	    	if(ret < 0)
	      		printf("No existing directory \n");
	    		clear();
	    		prompt();
	    	return;
	  }
	  if(i==0){
	  if (_inputFile == 0)
	{
		newfileinput = dup(defaultin);
	}
	else
	{
		newfileinput = open(_inputFile, O_RDONLY , 0666);
		if (newfileinput < 0)
		{
			printf("Error reading file");
			exit(2);
		}
	}}
		dup2(newfileinput, 0);
		close(newfileinput);
		if (i == _numberOfSimpleCommands - 1)
		{
			if (_outFile == 0)
			{
				newfileoutput = dup(defaultout);
			}
			else
			{
				if (flag == 1){
					
					newfileoutput = open(_outFile, O_WRONLY | O_APPEND | O_CREAT , 0666 );
					}
				else{
					
					newfileoutput = open(_outFile, O_WRONLY | O_CREAT , 0666 );
					}
				if (newfileoutput < 0)
				{
					printf("Error creating file");
					exit(2);
				}
			}
			if (_errFile == 0)
			{
				dup2(defaulterr, 2);
			}
			else
			{	if(flag==1){
				newfileerror = open(_errFile,O_WRONLY | O_APPEND | O_CREAT , 0666);

			}
			else{
					newfileerror = open(_errFile,O_WRONLY | O_CREAT , 0666);
				}

				
				if (newfileerror < 0)
				{
					printf("Error creating file");
					exit(2);
				}
				dup2(newfileerror, 2);
			}
		}
		else
		{
			int pipee[2];
			if(pipe(pipee)==-1){
				printf("error");
				exit(2);
			}
			pipe(pipee);
			newfileoutput = pipee[1];
			newfileinput = pipee[0];
		}
		dup2(newfileoutput, 1);
		close(newfileoutput);
		signal(SIGCHLD,logchild);
		pid = fork();
		if (pid == -1)
		{
			printf("Error");
			exit(2);
		}
		if (pid == 0)
		{
			int stat = execvp(_simpleCommands[i]->_arguments[0], _simpleCommands[i]->_arguments);
			if (stat == -1)
			{
				printf("Error in execution\n");
				exit(2);
			}

		}
		else {
			dup2( defaultin, 0 );
			dup2( defaultout, 1 );
			dup2( defaulterr, 2 );

		}

	}

	close(defaultin);
	close(defaultout);
	close(defaulterr);
	close(newfileinput);
	close(newfileoutput);

	if (_background == 0)
	{
		waitpid(pid, 0, 0);
	}
	else
	{
		// donot wait for child
	}
	//////////////////////////////////////////End od Execution Code /////////////////////////////////////////////////////////////

	// Clear to prepare for next command
	clear();

	// Print new prompt
	prompt();
}

// Shell implementation

void Command::prompt()
{
	printf("myshell>");
	

	fflush(stdout);

	
}

Command Command::_currentCommand;
SimpleCommand *Command::_currentSimpleCommand;

int yyparse(void);

int main()
{  
	signal(SIGINT,ctrlcfun);
	
	Command::_currentCommand.prompt();
	yyparse();
	return 0;
}

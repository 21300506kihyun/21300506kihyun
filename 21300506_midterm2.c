// If the comments are hard to read because of the color,
// type ':set background=dark'

/***

	This program reads texts and switches the case of the characters (lower case <-> upper case) until the user types "quit".

	Example)
		Input a text: Welcome!
		convered text = wELCOME!
		Input a text: God bless you!
		convered text = gOD BLESS YOU!
		Input a text: quit
		Bye!

	It launches a child process and communicates with it through two ordinary pipes, one to send the original text and the other to receive the converted text.

	Complete the program using ORDINARY PIPES by following the instructions.
	DO NOT use other IPC such as shared memory or message queue.

	The main creates two pipes and a child process.
	
	Then, the parent process repeats the followings until the user types "quit".
		Read a text line from the user.
		Send the text to the child through pipe.
		Receive and display the converted text.

	The child process repeats the followings until it receives "quit" from the parent. 
		Read a text line from the parent through one pipe.
		Convert all upper case characters to the corresponding lower case characters and vice versa.
		Write the converted text to the parent through the other pipe.

	Hint) To read a text line from the user, use the following code.
			printf("Input a text: ");
			fgets(src, BUFFER_SIZE - 1, stdin);	
			int len = strlen(src) - 1;
			src[len] = 0;				// trim '\n'

	Note! Before the parent terminates, there the child processing MUST terminate.
		You can check whether the child is still running by check currently running processes by 'ps -al'.

***/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0

#define READ_END 0
#define WRITE_END 1

#define BUFFER_SIZE 256

void SwitchCase(int in[], int out[]);

int main()
{
	int in[2];		// pipe from parent to child
	int out[2];		// pipe from child to parent

	char read_msg[255];
	char write_msg[255];
	int pid;

	if (pipe(in) < 0)
	{
			perror("pipe error : ");
			exit(0);
	}

	if (pipe(out) < 0)
	{
			perror("pipe error : ");
			exit(0);
	}

	if ((pid = fork()) < 0)
	{
			perror("fork error : ");
			exit(0);
	}
	//	TO DO: Create two ordinary pipes

	//  TO DO: Create child process

	else if( pid == 0){ //child process
				SwitchCase(in,out);
	}else{ //parent proess
			close(in[READ_END]); //단방향이므로 쓰기위한 파이프에서는 쓰는 기능 사용하지 않음
			close(out[WRITE_END]); // 단방향이므로 읽어오기 위한파이프에서는 쓰는기능 사용하지 않음
			while(1)
			{
				printf("Input a text: ");
				fgets(read_msg, BUFFER_SIZE - 1, stdin);	
				int len = strlen(read_msg) - 1;
				read_msg[len] = 0;				// trim '\n'
	
				write(in[WRITE_END], read_msg, strlen(read_msg)+1);

				if(strcmp(read_msg,"quit") == 0){
					wait(NULL); // wait for child process terminate
					close(in[WRITE_END]);
					close(out[READ_END]);
					printf("good-bye\n");
					break;
				}

				read(out[READ_END], write_msg, BUFFER_SIZE);
				printf("converted text = %s\n", write_msg);

			}

	}
	// On parent process,
		// Repeats the followings until the user types "quit".
			// Read a text line from the user.
			// Send the text to the child through pipe.
			// Receive and display the converted text.

		// Wait for the child to terminate
		// Deallocate pipes
		// Print a good-bye message


	// On child process call SwitchCase(in, out);

	return 0;
}

void SwitchCase(int in[], int out[])
{
		char buffer[BUFFER_SIZE];

		close(in[WRITE_END]); //단방향이므로 읽어오기 위한 파이프에서는 쓰는 기능 사용하지 않음
		close(out[READ_END]); //단방향이므로 부모프로세스에 쓰기위한 파이프에서는 읽기기능 사용하지 않음

		while(1){
				read(in[READ_END], buffer , BUFFER_SIZE);
				if(strcmp(buffer,"quit") == 0){
					close(in[READ_END]);
					close(out[WRITE_END]);
					break;
				}
				for(int i = 0; i < strlen(buffer); i++){
					if(isupper(buffer[i])){
						buffer[i] = tolower(buffer[i]);
					}
					else{
						buffer[i] = toupper(buffer[i]);
					}
				}

				write(out[WRITE_END], buffer, strlen(buffer)+1);

		}
	// TO DO: Implement the following algorithm
	// Repeats the followings until it receives "quit" from the parent. 
		//	Receive a text line from the parent through pipe in.
		//	Convert all upper case characters to the corresponding lower case characters and vice versa.
		// 		You may use isupper(), islower(), toupper(), tolower() functions.
		//		But DO NOT use other IPC such as shared memory or message queue.
		//	Send the converted text to the parent through pipe out.

	// TO DO: deallocate pipes;
}


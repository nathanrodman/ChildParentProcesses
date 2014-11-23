// Characters from command line arguments are sent to child process
// from parent process one at a time through pipe.
//
// Child process counts number of characters sent through pipe.
//
// Child process returns number of characters counted to parent process.
//
// Parent process prints number of characters counted by child process.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char **argv)
{
	// set up pipe
	pid_t pid;
	int comm[2];
	int status;
	char x;

	if (pipe(comm))
	{
		printf("pipe error\n");
		return -1;
	}

	// call fork()

	pid = fork();

	if (pid == 0) {
		// -- running in child process --
		int     nChars = 0;
		close(comm[1]);

		// Receive characters from parent process via pipe
		// one at a time, and count them.
		while( read(comm[0], &x, 1))
		{
			nChars++;
		}

		close(comm[0]);

		// Return number of characters counted to parent process.
		return nChars;
		}
	else {
		// -- running in parent process --
		int     nChars = 0;

		printf("CS201 - Assignment 3 - Nathan Rodman\n");

		// Send characters from command line arguments starting with
		// argv[1] one at a time through pipe to child process.
		close(comm[0]);
		
		int i = 1;
		while(i < argc)
		{
			int j = 0;
			while(argv[i][j]) 
			{
				write(comm[1], &(argv[i][j]), 1);
				j++;
			}
			i++;
		}
		close(comm[1]);


		// Wait for child process to return. Reap child process.
		// Receive number of characters counted via the value
		// returned when the child process is reaped.

		waitpid(pid, &nChars, 0);
		if(WIFEXITED(status))
			nChars = (WEXITSTATUS(status));
		nChars = nChars >> 010;
		printf("child counted %d characters\n", nChars);
		return 0;
		}
}

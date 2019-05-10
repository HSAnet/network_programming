/*
 * In-lecture task:
 * Write a small program that creates a socket (IPv4, UDP) and then waits
 * for input on stdin before exiting using the getchar() function (so that
 * the socket is created and you can inspect what has happened). In case you
 * forgot the information on the previous slide use man.
 *
 * Once your program runs and waits for input, check whether you can already
 * obtain information about the socket using the ss tool.
 *
 * Learning outcome:
 *    - Get familiar (again) with the tool chain
 *    - Create a first socket
 *    - Use tools to see what goes on at the OS level
 */


#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main()
{
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

	if(sock_fd < 0) {
		perror("Creating socket failed");
		exit(EXIT_FAILURE);
	}

	//wait for keyboard input
	getchar();

	close(sock_fd);

	return EXIT_SUCCESS;
}


/*
 * In-lecture task:
 * Write a small TCP client. connect to some server (e.g. www.hs-augsburg.de)
 * and do not just exit but wait for user input (e.g. with getchar() as before).
 * Observe with tcpdump, what kind of packets are being sent. Then exit the
 * program and observe what is being sent.
 *
 * Learning outcome:
 *    - Get familiar with connecting sockets
 */


#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

void
usage();

int
main()
{

	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(sock_fd < 0) {
		perror("Creating socket failed");
		exit(EXIT_FAILURE);
	}



	getchar();

	return EXIT_SUCCESS;
}

void
usage ()
{

	printf("Usage:\n\t04.out [DNS name]\n\n\
A program to connect to a TCP port\n\n");
	exit(EXIT_FAILURE);
}

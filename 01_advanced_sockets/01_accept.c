/*
 * Write a small server program based on what you already have programmed
 * so far that creates a socket and calls accept(). See what happens if the
 * socket is blocking and what happens when the socket is non-blocking.
 *
 * Learning outcome:
 *    - Understand the concept on non-blocking sockets and socket operations
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int
main(int argc, char *argv[])
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	struct addrinfo *res;

	hints.ai_flags = AI_PASSIVE;
	
	//hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_protocol = IPPROTO_TCP;

	int fail = 0;

	if((fail = getaddrinfo(NULL, "4422", &hints, &res)) != 0) {
		printf("%s\n", gai_strerror(fail));
		exit(EXIT_FAILURE);
	}

	struct addrinfo *tmp;

	//int sockfd = 0;

	char hbuf[NI_MAXHOST];
	//char sbuf[NI_MAXSERV];


	for(tmp = res; tmp != NULL; tmp = tmp->ai_next) {
		getnameinfo(tmp->ai_addr, tmp->ai_addrlen, hbuf, NI_MAXHOST,
		NULL, 0, 0);
		printf("%s\n", hbuf);
		       /*if((sockfd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol)) < 0) {
		       	perror("creating socket failed");
		           exit(EXIT_FAILURE);
		       } else {
		       	break;
		       } */
	}

	return EXIT_SUCCESS;
}
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
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>

void
usage();

int
main(int argc, char *argv[])
{

	if(argc != 3) {
		usage();
	}

	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));

	hints.ai_flags = AI_NUMERICSERV | AI_ADDRCONFIG;

	hints.ai_family = AF_UNSPEC;

	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo *lookup;

	if(getaddrinfo(argv[1], argv[2], NULL, &lookup)) {
		perror("DNS lookup failed");
		return(EXIT_FAILURE);
	}

	struct addrinfo *tmp = NULL;

	int sock_fd = 0;

	//picking the first that works might not what you want, but here you go...
	for(tmp = lookup; tmp != NULL; tmp = tmp->ai_next) {
		if((sock_fd = socket(tmp->ai_family, tmp->ai_socktype, 0)) < 0) {
			continue;
		} else {
			break;
		}
	}

	if(sock_fd < 0) {
		perror("Creating socket failed");
		freeaddrinfo(lookup);
		exit(EXIT_FAILURE);
	}

	//do the 3-way handshake
	if(connect(sock_fd, tmp->ai_addr, tmp->ai_addrlen)) {
		perror("TCP connection setup failed");
		freeaddrinfo(lookup);
		close(sock_fd);
		exit(EXIT_FAILURE);
	}

	getchar();

	freeaddrinfo(lookup);

	close(sock_fd);

	return EXIT_SUCCESS;
}

void
usage ()
{

	printf("Usage:\n\t04 [DNS name] [port number]\n\n\
A program to connect to a TCP server\n\n");
	exit(EXIT_FAILURE);
}

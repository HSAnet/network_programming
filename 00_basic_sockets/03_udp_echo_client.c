/*
 * In-lecture task:
 * Write a small program based on the last program. Take the dns name form the
 * command line argument and send the string “test” to that host to udp port 7
 * (Echo server port, https://tools.ietf.org/html/rfc862). Set the service
 * parameter of getaddrinfo to “7” in order to have the port set correctly
 * (consult the man page for the correct ai_flags to be set).
 *
 * Use tcpdump to see what your application sends out (and see if something
 * comes back!)
 *
 * Learning outcome:
 *    - Get familiar sending your first data and observe it with tcpdump
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

	if(argc != 2) {
		usage();
	}

	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));

	hints.ai_flags = AI_NUMERICSERV | AI_ADDRCONFIG;

	hints.ai_family = AF_UNSPEC;

	hints.ai_socktype = SOCK_DGRAM;

	struct addrinfo *lookup;

	if(getaddrinfo(argv[1], "7", &hints, &lookup)) {
		perror("DNS lookup failed");
		exit(EXIT_FAILURE);
	}

	char payload[] = "test";

	struct addrinfo *peer;

	int sockfd = 0;

	//this is not optimal since we pick the first and try...
	//what if the first fails (good enough for an example though)
	for(peer = lookup; peer != NULL; peer = peer->ai_next) {
		if((sockfd = socket(peer->ai_family, peer->ai_socktype, 0)) < 0) {
			continue;
		} else {
			break;
		}
	}

	if(sockfd <= 0) {
		printf("Failed to create a socket.\n");
		exit(EXIT_FAILURE);
	}

	int total_bytes_sent = 0;
	int bytes_sent = 0;
	int bytes_remaining = strlen(payload);

	while(total_bytes_sent < strlen(payload)) {
		if((bytes_sent = sendto(sockfd, payload + total_bytes_sent, bytes_remaining, 0,
		                        (struct sockaddr *)(peer->ai_addr), peer->ai_addrlen)) < 0) {
			perror("Writing to socket failed");
			break;
		}

		total_bytes_sent += bytes_sent;
		bytes_remaining -= bytes_sent;
	}

	//do not forget to free up the memory occupied by the result
	freeaddrinfo(lookup);

	close(sockfd);

	return EXIT_SUCCESS;
}


void
usage ()
{

	printf("Usage:\n\t03 [DNS name]\n\n\
A (trivial) UDP echo client\n\n");
	exit(EXIT_FAILURE);
}

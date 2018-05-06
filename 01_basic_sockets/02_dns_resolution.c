/*
 * In-lecture task:
 * Write a small program that does a DNS lookup of a DNS name entered as the
 * first (and only) command-line argument. Do not limit the address family
 * in hints, walk through the linked list of returned IPs and print them to
 * stdout. There are examples in the getaddrinfo man page at the bottom.
 *
 * Use tcpdump to see what your application sends out.
 * (Hint: use udp port 53 as a filter expression)
 *
 * Learning outcome:
 *    - Get familiar the DNS lookup part of network programming
 *    - Get used to use tcpdump to monitor your application
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

	//unused for now, but part of the homework...
	//you should play around with the service parameter, and the various
	//members in hints (see man page).
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));

	struct addrinfo *lookup;

	if(getaddrinfo(argv[1], NULL, NULL, &lookup)) {
		perror("DNS lookup failed");
		exit(EXIT_FAILURE);
	}

	struct addrinfo *tmp;

	char addr_str[INET6_ADDRSTRLEN];

	uint16_t port = 0;

	for(tmp = lookup; tmp != NULL; tmp = tmp->ai_next) {
		printf("---- Returned by getaddrinfo: ----\n");

		if(tmp->ai_family == AF_INET) {
			if(inet_ntop(AF_INET, &((struct sockaddr_in *)(tmp->ai_addr))->sin_addr,
			             addr_str, INET6_ADDRSTRLEN) == NULL) {
				continue;
			}

			port = ntohs(((struct sockaddr_in *)(tmp->ai_addr))->sin_port);
		} else if(tmp->ai_family == AF_INET6) {
			if(inet_ntop(AF_INET6, &((struct sockaddr_in6 *)(tmp->ai_addr))->sin6_addr,
			             addr_str, INET6_ADDRSTRLEN) == NULL) {
				continue;
			}

			port = ntohs(((struct sockaddr_in6 *)(tmp->ai_addr))->sin6_port);
		} else {
			printf("\tUnsupported address family\n");
			continue;
		}

		printf("\t%s : %d\n", addr_str, port);

		if(tmp->ai_socktype == SOCK_DGRAM) {
			printf("\tSocket type: SOCK_DGRAM\n");
		} else if(tmp->ai_socktype == SOCK_STREAM) {
			printf("\tSocket type: SOCK_STREAM\n");
		} else {
			printf("\tSocket type: UNKNOWN [%d]\n", tmp->ai_socktype);
		}

		if(tmp->ai_canonname != NULL) {
			printf("\tCanonical name: %s\n", tmp->ai_canonname);
		}
	}

	//do not forget to free up the memory occupied by the result
	freeaddrinfo(lookup);

	return EXIT_SUCCESS;
}


void
usage ()
{

	printf("Usage:\n\t02.out [DNS name]\n\n\
A (trivial) program to resolve DNS names\n\n");
	exit(EXIT_FAILURE);
}

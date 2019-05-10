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

#define RECV_BUFFER_SIZE 2048

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

	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo *lookup;

	if(getaddrinfo(argv[1], "80", NULL, &lookup)) {
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

	char http_get_stub[] = "GET / HTTP/1.1\r\nHost:";

	int http_get_size = strlen(http_get_stub) + strlen(argv[1]) + 5;
	char *http_get = malloc(http_get_size);
	memset(http_get, 0, http_get_size);

	strcpy(http_get, http_get_stub);
	strcpy(http_get + strlen(http_get_stub), argv[1]);
	strcpy(http_get + strlen(http_get_stub) + strlen(argv[1]), "\r\n\r\n");

	if(send(sock_fd, http_get, http_get_size, 0) < 0) {
		perror("send failed");
		close(sock_fd);
		freeaddrinfo(lookup);
		exit(EXIT_FAILURE);
	}

	char recv_buffer[RECV_BUFFER_SIZE];
	memset(recv_buffer, 0, RECV_BUFFER_SIZE);
	int bytes_received = 0;

	if((bytes_received = recv(sock_fd, recv_buffer, RECV_BUFFER_SIZE, 0)) < 0) {
		perror("recv failed");
		close(sock_fd);
		freeaddrinfo(lookup);
		free(http_get);
		exit(EXIT_FAILURE);
	}

	printf("%s", recv_buffer);

	free(http_get);

	freeaddrinfo(lookup);

	close(sock_fd);

	return EXIT_SUCCESS;
}

void
usage ()
{

	printf("Usage:\n\t05 [DNS name]\n\n\
A text-based browser of some sort\n\n");
	exit(EXIT_FAILURE);
}

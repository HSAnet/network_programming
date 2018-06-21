#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "tlv.h"

int main(int argc, char *argv[]) {

	if(argc != 2) {
		printf("Please provide an IPv4 address\n");
		exit(EXIT_SUCCESS);
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));

	if(inet_pton(AF_INET, argv[1], &addr.sin_addr) != 1) {
		perror("Address conversion failed");
		exit(EXIT_FAILURE);
	}

	printf("Connecting to %s\n", argv[1]);

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if(sockfd < 0) {
		perror("Unable to create socket");
		exit(EXIT_FAILURE);
	}

	struct tlv_header hdr;

	hdr.type = IP_ECHO_REQUEST;
	hdr.length = htons(sizeof(hdr));

	addr.sin_port = htons(4422);
	addr.sin_family = AF_INET;

	sendto(sockfd, &hdr, sizeof(hdr), 0, (struct sockaddr*) &addr, sizeof(addr));

	close(sockfd);

	return EXIT_SUCCESS;
}

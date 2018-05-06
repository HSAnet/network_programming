/*
 * In-lecture task:
 * Write a small program, that sends a TCP packet using raw sockets. Make it a
 * SYN packet and do not worry about the checksum. You can use sendto to
 * actually send the message. Look at the packet in Wireshark.
 *
 * Learning outcome:
 *    - First steps with raw sockets
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
#include <netinet/tcp.h>
#include <poll.h>
#include <linux/icmp.h>


void
usage();

int
main(int argc, char *argv[])
{

	if (argc > 1) {
		usage();
	}

	int sockfd = 0;

	if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0) {
		perror("Failed to create socket");
		exit(EXIT_FAILURE);
	}

	uint8_t packet[10000];
	memset(packet, 0, 10000);

	struct iphdr *ip = (struct iphdr *) packet;
	struct tcphdr *tcp = (struct tcphdr *) (packet + sizeof(*ip));

	struct pollfd fds[1];
	fds[0].fd = sockfd;
	fds[0].events = POLLIN;

	printf("Receiving TCP packets via raw sockets\n");

	int ret = 0;

	for(;;) {
		if((ret = poll(fds, 1, 10000)) == 0) {
			printf("Poll timed out, goodbye\n");
			exit(EXIT_SUCCESS); //not a failure really
		} else if (ret > 0) {

			if(recv(sockfd, packet, sizeof(packet), 0) < 0) {
				perror("recv failed");
				exit(EXIT_FAILURE);
			} else {
				printf("%#010x\n", htonl(tcp->seq));
			}
		} else {
			perror("Poll failed");
			exit(EXIT_FAILURE);
		}
	}

	close(sockfd);

	return EXIT_SUCCESS;
}

void
usage()
{

	printf("Usage:\n\t02\n");
	exit(EXIT_FAILURE);
}

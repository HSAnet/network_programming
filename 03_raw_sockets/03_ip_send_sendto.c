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

	if (argc != 2) {
		usage();
	}

	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;     //we only use a single IP version here to keep the code simple... should be v6 really

	struct addrinfo *lookup;

	if (getaddrinfo(argv[1], NULL, &hints, &lookup)) {
		perror("DNS lookup failed");
		exit(EXIT_FAILURE);
	}

	int sockfd = 0;

	if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
		perror("Failed to create socket");
		exit(EXIT_FAILURE);
	}

	/*	alternative
		int on = 1;
		if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof (on)) < 0) {
			perror("Could not set socket required socket option");
			exit(EXIT_FAILURE);
		}
	*/

	uint8_t packet[2048];
	memset(packet, 0, 2048);

	char *source_ip_str = "1.2.3.4";
	struct in_addr source_ip;

	if(inet_pton(AF_INET, source_ip_str, &source_ip) <= 0) {
		perror("Could not generate network representation of src IP");
		exit(EXIT_FAILURE);
	}

	struct iphdr *ip = (struct iphdr *) packet;

	ip->version = 4;

	ip->ttl = 10;

	ip->protocol = 6;

	ip->daddr = (u_int32_t)((struct sockaddr_in *)(lookup->ai_addr))->sin_addr.s_addr;

	ip->saddr = inet_addr("1.2.3.4");

	ip->ihl = 5;

	struct tcphdr *tcp = (struct tcphdr *) (packet + sizeof(*ip));

	tcp->syn = 1;

	tcp->source = htons(65535);

	tcp->dest = htons(4321);

	tcp->doff = 5;

	char addr_str[INET_ADDRSTRLEN];

	if(inet_ntop(lookup->ai_family, &((struct sockaddr_in *)(lookup->ai_addr))->sin_addr, addr_str, INET_ADDRSTRLEN) == NULL) {
		perror("Unable to create a string from the resolved IP address\n");
		exit(EXIT_FAILURE);
	}

	printf("Sending TCP packet via raw socket to %s\n", addr_str);

	if(sendto(sockfd, packet, sizeof(*ip) + sizeof(*tcp), 0, lookup->ai_addr, lookup->ai_addrlen) < 0) {
		perror("send failed");
		exit(EXIT_FAILURE);
	}

	//do not forget to free up the memory occupied by the result
	freeaddrinfo(lookup);

	close(sockfd);

	return EXIT_SUCCESS;
}

void
usage()
{

	printf("Usage:\n03_sendto.out [DNS name]\n\n\
A (trivial) program to send a TCP SYN packet\n\
Requires root permissions or cap_net_raw\n\n");
	exit(EXIT_FAILURE);
}

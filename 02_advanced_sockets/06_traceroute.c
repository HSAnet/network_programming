/*
 * In-lecture task:
 * Write a traceroute client using sendmsg to alter TTL values per message sent.
 * Use a tool such as Wireshark to look at how your local traceroute behaves
 * and copy that behaviour. You do not need to do time measurements, send
 * multiple messages per TTL etc. but try to mimic the behavior on the wire.
 * No fancy output required. Just try to see whether you got an ICMP error back
 * using IP_RECVERR.
 *
 * Learning outcome:
 *    - Learn how to use sendmsg and recvmsg to alter header information on a per-packet basis and to receive ancillary
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
#include <poll.h>
#include <linux/icmp.h>
#include <linux/errqueue.h>

#define TTL_MAX 30
#define RETRIES_MAX 3

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
	hints.ai_flags = AI_NUMERICSERV | AI_ADDRCONFIG;
	hints.ai_family = AF_INET;     //we only use a single IP version here to keep the code simple... should be v6 really
	hints.ai_socktype = SOCK_DGRAM;

	struct addrinfo *lookup;

	if (getaddrinfo(argv[1], "33438", &hints, &lookup)) {
		perror("DNS lookup failed");
		exit(EXIT_FAILURE);
	}

	char data[] = "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"; //taken from wireshark

	struct addrinfo *peer;

	int sockfd = 0;

	//this is not optimal since we pick the first and try...
	//what if the first fails (good enough for an example though)
	for (peer = lookup; peer != NULL; peer = peer->ai_next) {
		if ((sockfd = socket(peer->ai_family, peer->ai_socktype, 0)) < 0) {
			continue;
		} else {
			break;
		}
	}

	if (sockfd <= 0) {
		perror("Failed to create a socket, last error was\n");
		exit(EXIT_FAILURE);
	}

	int on = 1;
	if(setsockopt(sockfd, IPPROTO_IP, IP_RECVERR, &on, sizeof(on))) {
		perror("Unable to set socket option");
		exit(EXIT_FAILURE);
	}

	char addr_str[INET_ADDRSTRLEN];

	if(inet_ntop(peer->ai_family, &((struct sockaddr_in*)(peer->ai_addr))->sin_addr, addr_str, INET_ADDRSTRLEN) == NULL) {
		perror("Unable to create a string from the resolved IP address\n");
		exit(EXIT_FAILURE);
	}

	printf("Traceroute towards to %s\n", addr_str);

	//send data
	struct msghdr send_msg;
	memset(&send_msg, 0, sizeof(send_msg));

	struct iovec payload[1];
	payload[0].iov_base = data;
	payload[0].iov_len = strlen(data);

	send_msg.msg_name = peer->ai_addr;
	send_msg.msg_namelen = peer->ai_addrlen;

	send_msg.msg_iov = payload;
	send_msg.msg_iovlen = 1;

	//for setting the TTL
	struct cmsghdr *send_cmsg;
	int ttl = 1;
	int *ttl_ptr;
	uint8_t buf[CMSG_SPACE(sizeof ttl)];

	send_msg.msg_control = buf;
	send_msg.msg_controllen = sizeof(buf);

	send_cmsg = CMSG_FIRSTHDR(&send_msg);
	send_cmsg->cmsg_level = IPPROTO_IP;
	send_cmsg->cmsg_type = IP_TTL;
	send_cmsg->cmsg_len = CMSG_LEN(sizeof(ttl));

	ttl_ptr = (int *)CMSG_DATA(send_cmsg);
	*ttl_ptr = ttl;

	send_msg.msg_controllen = send_cmsg->cmsg_len;

	struct pollfd sockets[1];
	memset(&sockets, 0, sizeof(sockets));

	sockets[0].fd = sockfd;
	sockets[0].events = POLLIN | POLLERR;

	int ret = 0;
	int retries = 0;

	//----- Receive data

	struct msghdr recv_msg;
	memset(&recv_msg, 0, sizeof(recv_msg));

	char recv_buf[1500];
	char ctrl_buf[1024];

	struct sockaddr_storage src_addr;

	struct iovec recv_iov[1];
	recv_iov[0].iov_base = recv_buf;
	recv_iov[0].iov_len = sizeof(recv_buf);


	recv_msg.msg_name = &src_addr;
	recv_msg.msg_namelen = sizeof(src_addr);

	recv_msg.msg_iov = recv_iov;
	recv_msg.msg_iovlen=1;

	recv_msg.msg_control = ctrl_buf;
	recv_msg.msg_controllen = sizeof(ctrl_buf);

	struct cmsghdr *recv_cmsg;

	for (int n = 1; n <= TTL_MAX; ++n, ++(*ttl_ptr)) {
		if (sendmsg(sockfd, &send_msg, 0) < 0) {
			(*ttl_ptr)--;
			retries++;
			if(retries >= RETRIES_MAX) {
				fprintf(stderr, "Writing socket failed on ttl %d\n", (*ttl_ptr) + 1);
				perror("Writing socket failed");
			}
		}

		if ((ret = poll(sockets, 1, 1000)) < 0) {
			perror("Polling failed");
			exit(EXIT_FAILURE);
		} else if (ret > 0) {
			printf("ICMP packet received at TTL %d\n", *ttl_ptr);
			//there is only a single fd, so no need to look for the right one
			if(recvmsg(sockfd, &recv_msg, MSG_ERRQUEUE) < 0) {
				perror("recvmsg failed");
				exit(EXIT_FAILURE);
			}

			//warn in case buffers were too small
			if(recv_msg.msg_flags & MSG_CTRUNC || recv_msg.msg_flags & MSG_TRUNC) {
				printf("buffer to small to hold data/ancillary data\n");
			}

			for(recv_cmsg = CMSG_FIRSTHDR(&recv_msg); recv_cmsg != NULL; recv_cmsg = CMSG_NXTHDR(&recv_msg, recv_cmsg)) {
				if(recv_cmsg->cmsg_level == IPPROTO_IP && recv_cmsg->cmsg_type == IP_RECVERR) {
					struct sock_extended_err *ee = (struct sock_extended_err*)CMSG_DATA(recv_cmsg);

					if(ee->ee_origin == SO_EE_ORIGIN_ICMP && ee->ee_type == ICMP_TIME_EXCEEDED) {
						printf("ICMP time exceeded received\n");
					} else if(ee->ee_origin == SO_EE_ORIGIN_ICMP && ee->ee_type == ICMP_DEST_UNREACH) {
						printf("ICMP dest unreachable received... done\n");
						exit(EXIT_SUCCESS);
					}
				}

			}

		} else {
			printf("Recv timeout on ttl %d ...sending next packet\n", *ttl_ptr);
		}
	}

	//do not forget to free up the memory occupied by the result
	freeaddrinfo(lookup);

	close(sockfd);

	return EXIT_SUCCESS;
}

void 
usage()
{

	printf("Usage:\n\t06 [DNS name]\n");
	exit(EXIT_FAILURE);
}

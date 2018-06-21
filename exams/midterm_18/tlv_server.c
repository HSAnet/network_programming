#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#include "tlv.h"

int main(int argc, char *argv[]) {

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	struct addrinfo *res;

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_UDP;
	
	int fail = 0;

	if((fail = getaddrinfo(NULL, "4422", &hints, &res)) != 0) {
		printf("%s\n", gai_strerror(fail));
		exit(EXIT_FAILURE);
	}
	
	struct addrinfo *tmp;
	int sockfd = 0;	


	for(tmp = res; tmp != NULL; tmp = tmp->ai_next) {
		if((sockfd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol)) < 0) {
			perror("creating socket failed");
			exit(EXIT_FAILURE);
		} else {
			break;
		} 
	}

	if(bind(sockfd, tmp->ai_addr, tmp->ai_addrlen) != 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	struct pollfd poll_struct;
	memset(&poll_struct, 0, sizeof(poll_struct));

	poll_struct.fd = sockfd;
	poll_struct.events = POLLIN;

	struct tlv_header hdr;

	struct msghdr msg;
	memset(&msg, 0, sizeof(msg));

	struct sockaddr_in addr;

	struct tlv_header tlv;
	struct iovec io_data[1];
	io_data[0].iov_base = &tlv;
	io_data[0].iov_len = sizeof(tlv);

	msg.msg_iov = io_data;
	msg.msg_iovlen = 1;

	msg.msg_name = &addr;
	msg.msg_namelen = sizeof(addr);

	char addr_str[INET_ADDRSTRLEN];

	struct msghdr send_msg;
	memset(&send_msg, 0, sizeof(send_msg));
	struct iovec send_data[2];
	send_data[0].iov_base = &tlv;
	send_data[0].iov_len = sizeof(tlv);
	send_data[1].iov_base = &addr.sin_addr;
	send_data[1].iov_len = sizeof(addr.sin_addr);

	send_msg.msg_iov = send_data;
	send_msg.msg_iovlen = 2;

	send_msg.msg_name = &addr;
	send_msg.msg_namelen = sizeof(addr);

	int poll_ret = 0;

	for(;;) {
		poll_ret = poll(&poll_struct, 1, -1);

		if(poll_ret < 1) {
			perror("poll failed");
			exit(EXIT_FAILURE);
		} else if (poll_ret == 0) {
			printf("poll timed out\n");
		} else {
			int bytes = recvmsg(sockfd, &msg, 0);
			
			printf("Message received form %s\n", inet_ntop(AF_INET, &(((struct sockaddr_in*)msg.msg_name)->sin_addr), addr_str, INET_ADDRSTRLEN));
			printf("Type %d, length %d\n", tlv.type, tlv.length);
			tlv.type = IP_ECHO_RESPONSE;
			tlv.length = sizeof(tlv) + sizeof(addr);	
			bytes = sendmsg(sockfd, &send_msg, 0);
			if(bytes < 0) {
				perror("send failed");
			} else {
				printf("%d bytes sent\n", bytes);		
			}
		}
	}

	close(sockfd);

	return EXIT_SUCCESS;
}

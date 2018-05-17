#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <asm/types.h>
#include <linux/if_link.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{

	int sock_netlink = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);

	if (sock_netlink < 0) {
		perror("Unable to create an rt netlink socket");
		exit(EXIT_FAILURE);
	}

	//first, set the correct netlink multicast address
	//we want to be notified for route changes

	struct sockaddr_nl addr;
	memset(&addr, 0, sizeof(addr));
	addr.nl_family = AF_NETLINK;
	addr.nl_groups = RTMGRP_IPV4_ROUTE | RTMGRP_IPV6_ROUTE;

	char buffer[4096];

	bind(sock_netlink, (struct sockaddr *)&addr, sizeof(addr));

	struct nlmsghdr *nlhdr;
	int len = 0;
	struct rtmsg *routing_msg;

	//keep receiving
	for (;;) {
		if ((len = recv(sock_netlink, buffer, sizeof(buffer), 0)) > 0) {
			//we have a message!
			for (nlhdr = (struct nlmsghdr *)buffer; NLMSG_OK(nlhdr, len); nlhdr = NLMSG_NEXT(nlhdr, len)) {

				if (nlhdr->nlmsg_type == NLMSG_DONE)
					break;

				switch (nlhdr->nlmsg_type) {

				case RTM_NEWROUTE:
					printf("New Route ");
					break;

				case RTM_DELROUTE:
					printf("Delete Route ");
					break;

				case RTM_GETROUTE:
					printf("Get Route ");
					break;

				default:
					printf("Unknown ");
					break;
				}

				printf("message from %u [seq: %u, flags 0x%08x]\n", nlhdr->nlmsg_pid, nlhdr->nlmsg_seq, nlhdr->nlmsg_flags);

				routing_msg = (struct rtmsg *) NLMSG_DATA(nlhdr);

				if(routing_msg->rtm_family == AF_INET) {
					printf("\tIPv4 route entry\n");
				} else if(routing_msg->rtm_family == AF_INET6) {
					printf("\tIPv6 route entry\n");
				} else {
					printf("\t Unknown address family [%u]\n", routing_msg->rtm_family);
				}

			}
		} else {
			perror("receiving failed");
			exit(EXIT_FAILURE);
		}
	}

	close(sock_netlink);

	return EXIT_SUCCESS;
}

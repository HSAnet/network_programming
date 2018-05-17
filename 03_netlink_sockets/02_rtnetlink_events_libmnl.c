#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <asm/types.h>
#include <linux/if_link.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/socket.h>
#include <libmnl/libmnl.h>

int main(int argc, char *argv[])
{

	struct mnl_socket *nl_sock;

	nl_sock = mnl_socket_open(NETLINK_ROUTE);

	if (nl_sock == NULL) {
		perror("opening mnl socket failed");
		exit(EXIT_FAILURE);
	}

	if (mnl_socket_bind(nl_sock, RTMGRP_IPV4_ROUTE | RTMGRP_IPV6_ROUTE,
	                    MNL_SOCKET_AUTOPID) < 0) {
		perror("binding mnl socket failed");
		exit(EXIT_FAILURE);
	}

	char buf[MNL_SOCKET_BUFFER_SIZE];
	int ret = 0;

	for(;;) {
		ret = mnl_socket_recvfrom(nl_sock, buf, sizeof(buf));
		printf("received message of length %d\n", ret);
	}

	return EXIT_SUCCESS;
}

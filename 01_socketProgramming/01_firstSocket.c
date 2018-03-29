#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

int main() {

	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

	getchar();

	return 0;
}

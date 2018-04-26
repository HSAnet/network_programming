FLAGS = -Wall -Werror
COMPILER = clang

.PHONY: all clean style


all: 01.out 02.out 03.out

01.out: 01_first_socket.c
	$(COMPILER) $(FLAGS) -o 01.out 01_first_socket.c

02.out: 02_dns_resolution.c
	$(COMPILER) $(FLAGS) -o 02.out 02_dns_resolution.c

03.out: 03_udp_echo_client.c
	$(COMPILER) $(FLAGS) -o 03.out 03_udp_echo_client.c

style:
	astyle --style=linux --indent=tab 01_first_socket.c 02_dns_resolution.c 03_udp_echo_client.c

clean: 
	rm 01.out 02.out 03.out

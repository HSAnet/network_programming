FLAGS = -Wall -Werror
COMPILER = clang

.PHONY: all clean style

all: 01.out 02.out 03.out 04.out

01.out: 01_first_socket.c
	$(COMPILER) $(FLAGS) -o 01.out 01_first_socket.c

02.out: 02_dns_resolution.c
	$(COMPILER) $(FLAGS) -o 02.out 02_dns_resolution.c

03.out: 03_udp_echo_client.c
	$(COMPILER) $(FLAGS) -o 03.out 03_udp_echo_client.c

04.out: 04_tcp_connect.c
	$(COMPILER) $(FLAGS) -o 04.out 04_tcp_connect.c

style:
	astyle --style=linux --indent=tab --break-blocks --pad-oper \
	--align-pointer=name --align-reference=name \
	01_first_socket.c \
	02_dns_resolution.c \
	03_udp_echo_client.c \
	04_tcp_connect.c

clean: 
	rm *.out *.orig

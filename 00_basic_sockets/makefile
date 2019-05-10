FLAGS = -Wall
COMPILER = clang

.PHONY: all clean style

all: 01 02 03 04 05

01: 01_first_socket.c
	$(COMPILER) $(FLAGS) -o 01 01_first_socket.c

02: 02_dns_resolution.c
	$(COMPILER) $(FLAGS) -o 02 02_dns_resolution.c

03: 03_udp_echo_client.c
	$(COMPILER) $(FLAGS) -o 03 03_udp_echo_client.c

04: 04_tcp_connect.c
	$(COMPILER) $(FLAGS) -o 04 04_tcp_connect.c

05: 05_http_get.c
	$(COMPILER) $(FLAGS) -o 05 05_http_get.c

style:
	astyle --style=linux --indent=tab --break-blocks --pad-oper \
	--align-pointer=name --align-reference=name \
	01_first_socket.c \
	02_dns_resolution.c \
	03_udp_echo_client.c \
	04_tcp_connect.c \
	05_http_get.c

clean: 
	rm 01 02 03 04 05 *.orig

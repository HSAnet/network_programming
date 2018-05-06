FLAGS = -Wall -Werror
COMPILER = clang

.PHONY: style all clean

all: 01.out 02.out 03_sendto.out 03_sendmsg.out 04.out 05_recv.out \ 
	05_recvfrom.out

01.out: 01_tcp_syn.c
	$(COMPILER) $(FLAGS) -o 01.out 01_tcp_syn.c

02.out: 02_raw_recv.c
	$(COMPILER) $(FLAGS) -o 02.out 02_raw_recv.c

03_sendto.out: 03_ip_send_sendto.c
	$(COMPILER) $(FLAGS) -o 03_sendto.out 03_ip_send_sendto.c

03_sendmsg.out: 03_ip_send_sendmsg.c
	$(COMPILER) $(FLAGS) -o 03_sendmsg.out 03_ip_send_sendmsg.c

04.out: 04_icmp_spoof.c
	$(COMPILER) $(FLAGS) -o 04.out 04_icmp_spoof.c

05_recv.out: 05_packet_recv.c
	$(COMPILER) $(FLAGS) -o 05_recv.out 05_packet_recv.c

05_recvfrom.out: 05_packet_recvfrom.c
	$(COMPILER) $(FLAGS) -o 05_recvfrom.out 05_packet_recvfrom.c

style:
	astyle --style=linux --indent=tab --break-blocks --pad-oper \
	--align-pointer=name --align-reference=name \
	01_tcp_syn.c \
	02_raw_recv.c \
	03_ip_send_sendto.c \
	03_ip_send_sendmsg.c \
	04_icmp_spoof.c \
	05_packet_recv.c \
	05_packet_recvfrom.c

clean: 
	rm *.out *.orig

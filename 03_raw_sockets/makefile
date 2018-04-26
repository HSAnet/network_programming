FLAGS = -Wall -Werror
COMPILER = clang

.PHONY: style all clean


all: 01.out

01.out: 01_tcp_syn.c
	$(COMPILER) $(FLAGS) -o 01.out 01_tcp_syn.c

style:
	astyle --style=linux --indent=tab 01_tcp_syn.c

clean: 
	rm 01.out

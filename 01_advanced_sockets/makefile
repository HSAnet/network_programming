FLAGS = -Wall -Werror
COMPILER = clang

.PHONY: style all clean

all: 06.out

06.out: 06_traceroute.c
	$(COMPILER) $(FLAGS) -o 06.out 06_traceroute.c

style:
	astyle --style=linux --indent=tab --break-blocks --pad-oper \
	--align-pointer=name --align-reference=name \
	06_traceroute.c

clean: 
	rm *.out *.orig

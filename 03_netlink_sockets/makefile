FLAGS = -Wall -Werror
COMPILER = clang

.PHONY: style all clean

all: 01.out 02.out

01.out: 01_rtnetlink_events_raw.c
	$(COMPILER) $(FLAGS) -o 01.out 01_rtnetlink_events_raw.c

02.out: 02_rtnetlink_events_libmnl.c
	$(COMPILER) $(FLAGS) -lmnl -o 02.out 02_rtnetlink_events_libmnl.c

style:
	astyle --style=linux --indent=tab --break-blocks --pad-oper \
	--align-pointer=name --align-reference=name \
	01_rtnetlink_events_raw.c \
	02_rtnetlink_events_libmnl.c

clean: 
	rm *.out *.orig

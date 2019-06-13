FLAGS = -Wall
COMPILER = clang

.PHONY: all clean style

all: 01 02 03

01: 01_bio.c
	$(COMPILER) $(FLAGS) -o 01 -lcrypto -lssl 01_bio.c

02: 02_bio_chain.c
	$(COMPILER) $(FLAGS) -o 02 -lcrypto -lssl 02_bio_chain.c

03: 03_connect_bio.c
	$(COMPILER) $(FLAGS) -o 03 -lcrypto -lssl 03_connect_bio.c

style:
	astyle --style=linux --indent=tab --break-blocks --pad-oper \
	--align-pointer=name --align-reference=name \
	01_bio.c \
	02_bio_chain.c \
	03_connect_bio.c

clean: 
	rm 01 02 03 *.orig

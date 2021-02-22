SMASHER = ./bin/smasher
RSHELL = ./bin/reverse-shell
SH = ./bin/sh
VULNEABLE = ./bin/vulnerable

RM = rm -f

SMASHER_SRC ?= ./smasher/
SMASHER_CPP := $(shell find $(SMASHER_SRC) -name '*.cpp')

all: $(SMASHER) $(RSHELL) $(SH) $(VULNEABLE)

$(SMASHER): smasher/main.cpp
	g++ -o $(SMASHER) $(SMASHER_CPP) -m32 -I./smasher

$(RSHELL): payload/reverse-shell.asm
	nasm -o $(RSHELL) -fbin payload/reverse-shell.asm

$(SH): payload/sh.asm
	nasm -o $(SH) -fbin payload/sh.asm

$(VULNEABLE): vulnerable/main.c
	gcc -o $(VULNEABLE) vulnerable/main.c -m32 -fno-stack-protector -z execstack -w

clean:
	$(RM) $(SMASHER)
	$(RM) $(RSHELL)
	$(RM) $(SH)
	$(RM) $(VULNEABLE)

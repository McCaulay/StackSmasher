SMASHER = ./bin/smasher
RSHELL = ./bin/payloads/reverse-shell.bin
SH = ./bin/payloads/sh.bin
VULNEABLE = ./bin/vulnerable
XOR = ./bin/encoders/xor.bin

RM = rm -f

SMASHER_SRC ?= ./smasher/
SMASHER_CPP := $(shell find $(SMASHER_SRC) -name '*.cpp')

all: $(SMASHER) $(RSHELL) $(SH) $(VULNEABLE) $(XOR)

$(SMASHER): smasher/main.cpp
	g++ -o $(SMASHER) $(SMASHER_CPP) -m32 -I./smasher

$(RSHELL): payload/reverse-shell.asm
	nasm -o $(RSHELL) -fbin payload/reverse-shell.asm

$(SH): payload/sh.asm
	nasm -o $(SH) -fbin payload/sh.asm

$(VULNEABLE): vulnerable/main.c
	gcc -o $(VULNEABLE) vulnerable/main.c -m32 -fno-stack-protector -z execstack -w

$(XOR): encoders/xor.asm
	nasm -o $(XOR) -fbin encoders/xor.asm

clean:
	$(RM) $(SMASHER)
	$(RM) $(RSHELL)
	$(RM) $(SH)
	$(RM) $(VULNEABLE)
	$(RM) $(XOR)

SMASHER = ./bin/smasher
RSHELL = ./bin/payloads/reverse-shell.bin
BSHELL = ./bin/payloads/bind-shell.bin
SH = ./bin/payloads/sh.bin
SHUTDOWN = ./bin/payloads/shutdown.bin
VULNEABLE = ./bin/vulnerable
XOR = ./bin/encoders/xor.bin

RM = rm -f

SMASHER_SRC ?= ./smasher/
SMASHER_CPP := $(shell find $(SMASHER_SRC) -name '*.cpp')

all: $(SMASHER) $(RSHELL) $(BSHELL) $(SH) $(SHUTDOWN) $(VULNEABLE) $(XOR)

$(SMASHER): smasher/main.cpp
	g++ -o $(SMASHER) $(SMASHER_CPP) -m32 -std=c++17 -I./smasher

$(RSHELL): payload/reverse-shell.asm
	nasm -o $(RSHELL) -fbin payload/reverse-shell.asm

$(BSHELL): payload/bind-shell.asm
	nasm -o $(BSHELL) -fbin payload/bind-shell.asm

$(SH): payload/sh.asm
	nasm -o $(SH) -fbin payload/sh.asm

$(SHUTDOWN): payload/shutdown.asm
	nasm -o $(SHUTDOWN) -fbin payload/shutdown.asm

$(VULNEABLE): vulnerable/main.c
	gcc -o $(VULNEABLE) vulnerable/main.c -m32 -fno-stack-protector -z execstack -no-pie

$(XOR): encoders/xor.asm
	nasm -o $(XOR) -fbin encoders/xor.asm

clean:
	$(RM) $(SMASHER)
	$(RM) $(RSHELL)
	$(RM) $(BSHELL)
	$(RM) $(SH)
	$(RM) $(SHUTDOWN)
	$(RM) $(VULNEABLE)
	$(RM) $(XOR)

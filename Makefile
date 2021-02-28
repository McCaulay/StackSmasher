SMASHER = ./bin/smasher
RSHELL = ./bin/payloads/reverse-shell.bin
BSHELL = ./bin/payloads/bind-shell.bin
EXECUTE = ./bin/payloads/execute.bin
SHUTDOWN = ./bin/payloads/shutdown.bin
ARGUMENT = ./bin/vulnerable-argument
REMOTE = ./bin/vulnerable-remote
XOR = ./bin/encoders/xor.bin

RM = rm -f

SMASHER_SRC ?= ./smasher/
SMASHER_CPP := $(shell find $(SMASHER_SRC) -name '*.cpp')

all: $(SMASHER) $(RSHELL) $(BSHELL) $(EXECUTE) $(SHUTDOWN) $(ARGUMENT) $(REMOTE) $(XOR)

$(SMASHER): smasher/main.cpp
	g++ -o $(SMASHER) $(SMASHER_CPP) -m32 -std=c++17 -I./smasher

$(RSHELL): payload/reverse-shell.asm
	nasm -o $(RSHELL) -fbin payload/reverse-shell.asm

$(BSHELL): payload/bind-shell.asm
	nasm -o $(BSHELL) -fbin payload/bind-shell.asm

$(EXECUTE): payload/execute.asm
	nasm -o $(EXECUTE) -fbin payload/execute.asm

$(SHUTDOWN): payload/shutdown.asm
	nasm -o $(SHUTDOWN) -fbin payload/shutdown.asm

$(ARGUMENT): vulnerable/argument.c
	gcc -o $(ARGUMENT) vulnerable/argument.c -m32 -fno-stack-protector -z execstack -no-pie

$(REMOTE): vulnerable/remote.c
	gcc -o $(REMOTE) vulnerable/remote.c -m32 -fno-stack-protector -z execstack -no-pie

$(XOR): encoders/xor.asm
	nasm -o $(XOR) -fbin encoders/xor.asm

clean:
	$(RM) $(SMASHER)
	$(RM) $(RSHELL)
	$(RM) $(BSHELL)
	$(RM) $(EXECUTE)
	$(RM) $(SHUTDOWN)
	$(RM) $(ARGUMENT)
	$(RM) $(REMOTE)
	$(RM) $(XOR)

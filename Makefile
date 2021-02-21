SMASHER = ./bin/smasher
PAYLOAD = ./bin/payload
VULNEABLE = ./bin/vulnerable

RM = rm -f

SMASHER_SRC ?= ./smasher/
SMASHER_CPP := $(shell find $(SMASHER_SRC) -name '*.cpp')

all: $(SMASHER) $(PAYLOAD) $(VULNEABLE)

$(SMASHER): smasher/main.cpp
	g++ -o $(SMASHER) $(SMASHER_CPP) -m32 -I./smasher

$(PAYLOAD): payload/payload.asm
	nasm -o $(PAYLOAD) -fbin payload/payload.asm

$(VULNEABLE): vulnerable/main.c
	gcc -o $(VULNEABLE) vulnerable/main.c -m32 -fno-stack-protector -z execstack -w

clean:
	$(RM) $(SMASHER)
	$(RM) $(PAYLOAD)
	$(RM) $(VULNEABLE)

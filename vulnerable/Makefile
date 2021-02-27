CC = gcc
RM = rm -f
ARGUMENT = ../bin/vulnerable-argument
REMOTE = ../bin/vulnerable-remote

all: $(ARGUMENT) $(REMOTE)

$(ARGUMENT): argument.c
	gcc -o $(ARGUMENT) argument.c -m32 -fno-stack-protector -z execstack -no-pie

$(REMOTE): remote.c
	gcc -o $(REMOTE) remote.c -m32 -fno-stack-protector -z execstack -no-pie

clean:
	$(RM) $(ARGUMENT) $(REMOTE)
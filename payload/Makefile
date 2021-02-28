RM = rm -f
RSHELL = ../bin/payloads/reverse-shell.bin
BSHELL = ../bin/payloads/bind-shell.bin
EXECUTE = ../bin/payloads/execute.bin
SHUTDOWN = ../bin/payloads/shutdown.bin

all: $(RSHELL) $(SH)

$(RSHELL): reverse-shell.asm
	nasm -o $(RSHELL) -fbin reverse-shell.asm

$(BSHELL): bind-shell.asm
	nasm -o $(BSHELL) -fbin bind-shell.asm

$(EXECUTE): execute.asm
	nasm -o $(EXECUTE) -fbin execute.asm

$(SHUTDOWN): shutdown.asm
	nasm -o $(SHUTDOWN) -fbin shutdown.asm

clean:
	$(RM) $(RSHELL)
	$(RM) $(BSHELL)
	$(RM) $(EXECUTE)
	$(RM) $(SHUTDOWN)
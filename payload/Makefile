RM = rm -f
RSHELL = ../bin/payloads/reverse-shell.bin
BSHELL = ../bin/payloads/bind-shell.bin
SH = ../bin/payloads/sh.bin

all: $(RSHELL) $(SH)

$(RSHELL): reverse-shell.asm
	nasm -o $(RSHELL) -fbin reverse-shell.asm

$(BSHELL): bind-shell.asm
	nasm -o $(BSHELL) -fbin bind-shell.asm

$(SH): sh.asm
	nasm -o $(SH) -fbin sh.asm

clean:
	$(RM) $(RSHELL)
	$(RM) $(BSHELL)
	$(RM) $(SH)
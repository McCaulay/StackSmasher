RM = rm -f
RSHELL = ../bin/reverse-shell
SH = ../bin/sh

all: $(RSHELL) $(SH)

$(RSHELL): reverse-shell.asm
	nasm -o $(RSHELL) -fbin reverse-shell.asm

$(SH): sh.asm
	nasm -o $(SH) -fbin sh.asm

clean:
	$(RM) $(RSHELL)
	$(RM) $(SH)
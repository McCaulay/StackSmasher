RM = rm -f
XOR = ../bin/encoders/xor.bin

all: $(XOR)

$(XOR): xor.asm
	nasm -o $(XOR) -fbin xor.asm

clean:
	$(RM) $(XOR)
RM = rm -f
OUT = ../bin/payload

$(OUT): payload.asm
	nasm -o $(OUT) -fbin payload.asm

clean:
	$(RM) $(OUT)
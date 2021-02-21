CC = gcc
RM = rm -f
OUT = ../bin/vulnerable

$(OUT): main.c
	gcc -o $(OUT) main.c -m32 -fno-stack-protector -z execstack

clean:
	$(RM) $(OUT)
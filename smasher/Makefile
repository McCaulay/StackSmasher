CC = g++
RM = rm -f
OUT = ../bin/smasher

SRC ?= ./
CPP := $(shell find $(SRC) -name '*.cpp')

$(OUT): main.cpp
	$(CC) -o $(OUT) $(CPP) -m32 -I.

clean:
	$(RM) $(OUT)
HEADERS = headers/Stack.h headers/Text.h headers/ASSert.h headers/enum.h


all: assembler processor disassembler clean

%.o: %.cpp $(HEADERS)
	g++ -c $< -o $@

assembler: ASSEMBLER.o
	g++ $^ -o $@

processor: CPU.o Stack_func.o
	g++ $^ -o $@

disassembler: DISASSEMBLER.o
	g++ $^ -o $@ 

clean:
	rm -rf *.o

run: asm cpu

asm:
	./assembler

cpu:
	./processor

disasm:
	./disassembler

HEADERS = headers/ASSert.h headers/Text.h headers/functions.h headers/FuncTree.h headers/constants.h

all:lang clean

lang: lang.o func.o
	g++ $^ -o lang

func.o: $(HEADERS)
	g++ -c headers/func.cpp -o $@

lang.o: lang.cpp $(HEADERS)
	g++ -c $< -o $@

clean:
	rm -rf *.o
	

toasm:
	temp/./assembler
	temp/./processor



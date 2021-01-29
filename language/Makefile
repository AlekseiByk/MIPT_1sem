HEADERS = headers/ASSert.h headers/Text.h headers/functions.h headers/FuncTree.h

all:lang remove

lang: lang.o func.o
	g++ $^ -o lang

func.o: $(HEADERS)
	g++ -c headers/func.cpp -o $@

lang.o: lang.cpp $(HEADERS)
	g++ -c $< -o $@

remove:
	rm -rf lang.o
	rm -rf func.o
	

toasm:
	temp/./assembler
	temp/./processor



HEADERS = headers/functions.h headers/Text.h headers/ASSert.h headers/FuncTree.h


all: obhod clean
	
%.o: %.cpp $(HEADERS)
	g++ -c $< -o $@

obhod: func.o obhod.o 
	g++ $^ -o obhod

clean:
	rm -rf func.o
	rm -rf obhod.o

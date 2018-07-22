SRC = src
INC = header
OBJ = obj
BIN = bin
CXX = g++ -std=c++14 -O3
CPPFLAGS = -w -g  -I$(INC) -c


all: $(BIN)/cse

documentacion:
	doxygen doc/doxys/Doxyfile

run: all
	$(shell ./testing.sh)


# ************ Compilación de módulos ************
$(BIN)/cse: $(OBJ)/main.o $(OBJ)/scanner.o $(OBJ)/structs.o $(OBJ)/stree.o $(OBJ)/cstree.o
	$(CXX) -o $(BIN)/cse $^ -I$(INC)

$(OBJ)/structs.o: $(SRC)/structs.cpp $(INC)/structs.h
	$(CXX) $(CPPFLAGS)  -o $(OBJ)/structs.o $(SRC)/structs.cpp

$(OBJ)/stree.o: $(SRC)/SyntaxTree.cpp   $(INC)/SyntaxTree.h
	$(CXX) $(CPPFLAGS)  -o $(OBJ)/stree.o $(SRC)/SyntaxTree.cpp

$(OBJ)/scanner.o: $(SRC)/Scanner.cpp $(INC)/Scanner.h
	$(CXX) $(CPPFLAGS)  -o $(OBJ)/scanner.o $(SRC)/Scanner.cpp

$(OBJ)/cstree.o: $(SRC)/CseTree.cpp $(INC)/CseTree.h $(OBJ)/stree.o
	$(CXX) $(CPPFLAGS)  -o $(OBJ)/cstree.o $(SRC)/CseTree.cpp

$(OBJ)/main.o: $(SRC)/main.cpp
	$(CXX) $(CPPFLAGS)  -o $(OBJ)/main.o $(SRC)/main.cpp

# ************ Limpieza ************
clean :
	-rm -rf $(OBJ)/* $(BIN)/* output/*

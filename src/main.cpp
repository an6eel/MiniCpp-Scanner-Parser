#include <fstream>
#include <iostream>
#include <string.h>
#include "Scanner.h"
#include "SyntaxTree.h"
#include "CseTree.h"

using namespace std;

void printToken(TokenType type, std::list<std::string>::iterator &var){
	cout <<  tokenString(type) << " ";
	if(type == NUM || type == ID){
		cout << (*var) << endl;
		var++;
	}else{
		cout << endl;
	}
}

int main(int argc, char *argv[]) {

	if(argc < 2){
		cerr << "Number of arguments incorrect" << endl;
		exit(-1);
	}

	ifstream in(argv[1]);

	string file=string(argv[1]);
	size_t pos=file.rfind('/');
	if(pos!=string::npos)
		file=file.substr(pos+1,file.size());

	string fileout="./output/"+ file+".out";
	ofstream out(fileout);

	if(in.bad()){
		cerr << "File incorrect or not exist" << endl;
		exit(-1);
	}
	Scanner scanner;
	scanner.parse(in);


	if(*(scanner.tokens.rbegin())==ERROR)
		exit(-1);

	TokenType *tokens = new TokenType[scanner.tokens.size()];
	int* lines =new int[scanner.lines.size()];
	int len = 0;
	auto i = scanner.tokens.begin();
	auto j=scanner.lines.begin();

	for(i,j; i != scanner.tokens.end(); i++,j++){
		if(*i != COMMENT){
			tokens[len] = *i;
			lines[len++]=*j;
		}
	}

	CseTree cstree(tokens,scanner.vars.begin(),lines);

	//cstree.print();
	cstree.ApplyCSE();
	//cstree.print();

	if(out.bad())
		cstree.printCode(std::cout);
	else
		cstree.printCode(out);

	in.close();
	out.close();
	delete[] tokens;

	return 0;
}

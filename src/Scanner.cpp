/**
	*  @file  Scanner.cpp
	*  @author  Ángel García Malagón
	*  @version 1.0
	*
	* @brief File with implementation of Scanner class
	*
	*/

#include "Scanner.h"
#include <list>
#include <string.h>
#include <iostream>


const int Scanner::KEYWORD_MAXLENGTH = 7;
const char *Scanner::KeywordList[3] = {
	"int",
	"return",
	"void",
};
const int Scanner::VAR_MAXLENGTH = 32;

TokenType Scanner::StringToKeyword(const char *word){
	for(int i = 0; i < 3; i++){
		if(strcmp(word, KeywordList[i]) == 0){
			switch(i){
				case 0:return INT;
				case 1:return RETURN;
				case 2:return VOID;
			}
		}
	}

	return ID;
}

bool Scanner::IsSpace(char ch){
	return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

bool Scanner::IsLetter(char ch){
	return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

bool Scanner::IsDigit(char ch){
	return '0' <= ch && ch <= '9';
}

void Scanner::parse(std::ifstream& in){
	tokens.clear();
	vars.clear();
	lines.clear();
	_current_line=1;

	TokenStatus status = Start;
	char word[VAR_MAXLENGTH];
	int wi = 0;

	char ch;
	while((ch=in.get())!= EOF){
		switch(status){
				case Start:
					while(IsSpace(ch)){
						if(ch=='\n')
							_current_line++;
						ch=in.get();
					}

					if(IsLetter(ch)){
						status = InID;
						word[wi++] = ch;

					}else if(IsDigit(ch)){
						status = InNumber;
						word[wi++] = ch;

					}else if(ch == '+'){
						tokens.push_back(PLUS);
						lines.push_back(_current_line);
					}else if(ch == '-'){
						tokens.push_back(MINUS);
						lines.push_back(_current_line);
					}else if(ch == '*'){
						tokens.push_back(MULTI);
						lines.push_back(_current_line);
					}else if(ch == '/'){
						ch=in.get();
						if(ch == '*'){
							status = InComment;
						}else{
							tokens.push_back(DIV);
							lines.push_back(_current_line);
							in.unget();
						}

					}else if(ch == '='){
						tokens.push_back(ASSIGN);
						lines.push_back(_current_line);
					}else if(ch == ';'){
						tokens.push_back(SEMICOLON);
						lines.push_back(_current_line);
					}else if(ch == ','){
						tokens.push_back(COMMA);
						lines.push_back(_current_line);
					}else if(ch == '('){
						tokens.push_back(LEFT_PARENTHESE);
						lines.push_back(_current_line);
					}else if(ch == ')'){
						tokens.push_back(RIGHT_PARENTHESE);
						lines.push_back(_current_line);
					}else if(ch == '{'){
						tokens.push_back(LEFT_BRACE);
						lines.push_back(_current_line);
					}else if(ch == '}'){
						tokens.push_back(RIGHT_BRACE);
						lines.push_back(_current_line);
					}
					else{
						if((int)ch!=-1){
							std::cerr << "Line " << _current_line << "-> Lexical error: " << ch << " isn't a valid element of my lenguage." << std::endl;
							tokens.push_back(ERROR);
							return;
						}
					}
			break;

			case InID:
				if(IsLetter(ch)){
					if(wi < VAR_MAXLENGTH - 1){
						word[wi++] = ch;
					}
				}else{
					word[wi] = 0;
					wi = 0;

					TokenType keyword = StringToKeyword(word);
					tokens.push_back(keyword);
					lines.push_back(_current_line);
					if(keyword == ID){
						vars.push_back(std::string(word));
					}

					status = Start;
					in.unget();
				}

			break;

			case InNumber:
				if(IsDigit(ch)){
					if(wi < VAR_MAXLENGTH - 1){
						word[wi++] = ch;
					}
				}else{
					word[wi] = 0;
					wi = 0;
					vars.push_back(std::string(word));

					tokens.push_back(NUM);
					lines.push_back(_current_line);

					status = Start;
					in.unget();
				}
			break;

			case InComment:
				if(ch=='\n')
					_current_line++;
				if(ch == '*'){
					ch=in.get();

					if(ch == '/'){
						tokens.push_back(COMMENT);
						lines.push_back(_current_line);
						status = Start;
					}
				}

			break;

			default:;
		}
	}

	status = End;
	tokens.push_back(ENDFILE);
	lines.push_back(_current_line);
}

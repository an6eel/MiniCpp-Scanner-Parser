/**
	*  @file  Scanner.cpp
	*  @author  Ángel García Malagón
	*  @version 1.0
	*
	* @brief File with implementation of functions declared in structs.h
	*
	*/

#include "structs.h"
#include <string>

std::string tokenString(TokenType token){
	switch(token){
		case ENDFILE:return "ENDFILE";break;
		case ERROR:return "ERROR";break;

		case INT:return "INT";break;
		case RETURN:return "RETURN";break;
		case VOID:return "VOID";break;

		case ASSIGN:return "ASSIGN";break;

		case SEMICOLON:return "SEMICOLON";break;
		case COMMA:return "COMMA";break;
		case LEFT_PARENTHESE:return "LEFT_PARENTHESE";break;
		case RIGHT_PARENTHESE:return "RIGHT_PARENTHESE";break;
		case LEFT_BRACE:return "LEFT_BRACE";break;
		case RIGHT_BRACE:return "RIGHT_BRACE";break;
		case COMMENT:return "COMMENT";break;

		case PLUS:return "PLUS";break;
		case MINUS:return "MINUS";break;
		case MULTI:return "MULTI";break;
		case DIV:return "DIV";break;
		case NUM:return "NUM";break;
		case ID:return "ID";break;
	}
}

std::string operatorString(TokenType token){
	switch(token){
    case PLUS:return "+";break;
		case MINUS:return "-";break;
		case MULTI:return "*";break;
		case DIV:return "/";break;
    case ASSIGN:return "=";break;
  }
}

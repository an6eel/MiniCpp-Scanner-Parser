/**
 *  @file  Structs.h
 *  @author  Ángel García Malagón
 *  @version 1.0
 *
 *  @brief List of Token types and some functions to work with tokens
 *
 *
 **/
#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <list>
#include <string>

/**
	* \enum TokenType
	* @brief Enum of possibles types of token allowed by the Scanner
	*/

enum TokenType{
	ENDFILE,

	ERROR,
	EMPTY,

	INT,
	RETURN,
	VOID,

	ASSIGN,

	SEMICOLON,
	COMMA,
	LEFT_PARENTHESE,
	RIGHT_PARENTHESE,
	LEFT_BRACE,
	RIGHT_BRACE,
	COMMENT,

	PLUS,
	MINUS,
	MULTI,
	DIV,

	NUM,
	ID
};

#ifndef NULL
#define NULL 0
#endif

typedef std::list<std::string> StringList;

/**
	* @brief Transforms token to String
	* @param token token
	* @return const char* Token string
	*/

std::string tokenString(TokenType token);

/**
	* @brief Transforms operator token to String
	* @param token token
	* @return string operator token string
	*/

std::string operatorString(TokenType token);

#endif

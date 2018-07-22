/**
 *  @file  Scanner.h
 *  @author  Ángel García Malagón
 *  @version 1.0
 *
 *  @brief Obtain token list and variable list of code
 *
 *  The lexer of my lenguage is the set of different types of tokens available in the lenguage
 *
 * The set of tokens is:
 **********************************************************************************************

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

 ************************************************************************************************
 *
 * This list is defined in the file structs.h
 *
 * The Scanner goal is to check that all elements of input code correspond to a valid token of my lenguage\n
 * If any element don't correspond with a valid token, Scanner throws a Lexical Error
 *
 **/

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "structs.h"
#include <fstream>



/**
	* \class Scanner
	* @brief Performs a lexical analysis of code
	*
	**/

class Scanner{
protected:
	static const int KEYWORD_MAXLENGTH; //< Max length of a keyword
	static const char *KeywordList[3]; //< List of keywords
	static const int VAR_MAXLENGTH; //< Max length of a variable

public:

	/**
		* \enum TokenStatus
		* @brief Status of scanner in each moment
		*
		**/

	enum TokenStatus{
		Start, InComment, InID, InNumber, End
	};

	/**
		* @brief Transforms a keyword to a Token
		* @param word Keyword
		* @return TokenType Keyword Token
		*/

	static TokenType StringToKeyword(const char * word);

	/**
		* @brief Check if ch is a space
		* @retval True if ch is a space
		* @retval False if ch isn't a space
		*/

	static bool IsSpace(char ch);

	/**
		* @brief Check if ch is a letter
		* @retval True if ch is a letter
		* @retval False if ch isn't a letter
		*/

	static bool IsLetter(char ch);

	/**
		* @brief Check if ch is a digit
		* @retval True if ch is a digit
		* @retval False if ch isn't a digit
		*/

	static bool IsDigit(char ch);

	std::list<TokenType> tokens; //< List of tokens
	StringList vars; //< List of variables
	std::list<int> lines; //< List with the line of each token
	int _current_line; //< Current line of the input file

	/**
		* @brief Parse a input stream(code ) getting the list of tokens and variables
		* @param in input stream
		*/

	void parse(std::ifstream& in);
};

#endif

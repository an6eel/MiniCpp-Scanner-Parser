/**
 *  @file  SyntaxTree.h
 *  @author  Ángel García Malagón
 *  @version 1.0
 *
 *  @brief SyntaxTree
 *
 *  SyntaxTree of code, but with common subexpressions eliminated

 *  The syntax of my lenguage is:
 **********************************************************************************************

	<PROGRAM> 		-> 	<DECLARATION>
	<DECLARATION>		-> 	<TYPE> ID LEFT_PARENTHESE RIGHT_PARENTHESE <COMPOUNT_STMT>
 	<TYPE> 			->  	VOID | INT
	<COMPOUNT_STMT> 	 	-> 	LEFT_BRACE <LOCAL_DECLARATIONS> <STATEMENT_LIST> RIGHT_BRACE
	<LOCAL_DECLARATIONS>     -> 	<VAR_DECLARATION>*
	<VAR_DECLARATION>	->      <TYPE> ID SEMICOLON
	<STATEMENT_LIST>		-> 	<STATEMENT>*
	<STATEMENT>		->      <RETURN_STMT> | <EXPRESSION_STMT>
	<EXPRESSION_STMT>	-> 	<ASSIGN_EXPRESSION> SEMICOLON
	<RETURN_STMT>		-> 	RETURN <EXPRESSION> SEMICOLON
	<ASSIGN_EXPRESSION>	->      <FACTOR> ASSIGN <SIMPLE_EXPRESSION>
	<SIMPLE_EXPRESSION> 	-> 	<TERM> ( <ADDOP> <TERM> )*
	<ADDOP> 			-> 	PLUS | MINUS
	<TERM>			-> 	<FACTOR> ( <MULOP> <FACTOR> )*
	<MULOP> 			-> 	MULTI | DIV
	<FACTOR> 		-> 	LEFT_PARENTHESE <EXPRESSION> RIGHT_PARENTHESE | ID | NUM

 **********************************************************************************************

 \n
 *	 Following the previous syntax, SyntaxTree will be construct.\n
 * 	  The Syntax Analyzer is to check if the input code follows the syntax of my lenguage
 *	  This means that the code follows the syntax productions shown above\n
 *    If Syntax Analyzer throws no error, the SyntaxTree will be created.
 *
 *
 **/

#ifndef __SYNTAXTREE_H__
#define __SYNTAXTREE_H__

#include "structs.h"
#include <unordered_set>

/**
* \class SyntaxTree
* @brief Create a SyntaxTree from list of tokens and list of variables
*
**/

class SyntaxTree{
public:
	/**
		* @brief Construct a SyntaxTree
		* @param tokens List of tokens of code
		* @param vars List of variables of code
		*/

	SyntaxTree(const TokenType *tokens, StringList::iterator vars,const int* lines);

	/// Destructor of SyntaxTree
	~SyntaxTree();

	/**
	* @brief create a SyntaxTree
	* @param tokens List of tokens of code
	* @param vars List of variables of code
	* @retval True if no error occurred
	* @retval False if any error occurred
	*/

	bool parse(const TokenType *tokens, StringList::iterator vars);

	/**
		* @brief print the SyntaxTree
		*/

	void print();

	/**
	* \class Node
	* @brief Node of SyntaxTree
	*
	**/

	class Node{
	public:
		/**
			@brief Constructor of node
			@param type Node type
			*/

		Node(TokenType type = EMPTY);

		/**
			@brief Copy Constructor of node
			@param node Node to copy
			*/

		Node(Node * node );

		/// Destructor of Node
		~Node();

		/**
			* @brief Set the type of Node
			* @param type Type of Node
			*/

		void setType(TokenType type);

		/**
			* @brief Get the type of Node
			* @return TokenType type of Node
			*/


		TokenType type() const;

		Node *child;//< Child of node

		/**
			* @brief Set the child of Node
			* @param type Type of Node
			* @return Node* child of Node
			*/

		Node *setChild(TokenType);

		Node *sibling;//< Sibling of node

		/**
			* @brief Set the sibling of Node
			* @param type Type of Node
			* @return Node* sibling of Node
			*/

		Node *setSibling(TokenType);


		std::string value; //< Value of node

	protected:
		TokenType _type; //< Type of node
	};

private:

	/**
		* @brief Handle Syntax errors
		* @param message Error message
		*/

	void error(std::string message);

	/**
		* @brief Print the SyntaxTree from current node with Depth equal to depth
		* @param current node to print
		* @param depth Depth to printed tree
		*/

	void print(Node *current, int depth);

	/**
		* @brief Get the current token
		* @return TokenType current token
		*/

	TokenType currentToken();

	/**
		* @brief Get the next token
		* @return TokenType next token
		*/
	TokenType forwardToken();

	/**
		* @brief Set the current token to next one
		*/
	void nextToken();

	/**
		* @brief Set the current token to previous one
		*/

	void prevToken();

	/**
		* @brief Get the current token
		* @return TokenType* current token
		*/

	const TokenType *tokenPosition();

	/**
	  * @brief Set the 'current token' to token
		* @param token token position
		*/

	void setTokenPosition(const TokenType * token);

	/**
		* @brief Check if token is equal to current token
		* @param token Token
		* @retval True if token is equal to current token
		* @retval False if token isn't equal to current token
		*/

	bool matchToken(TokenType token);

	/**
		* @brief Get the current variable
		* @return string Current variable
		*/

	std::string getVar();

	/// <PROGRAM> production

	Node *program();

	/// <DECLARATION> production

	Node *declaration();

	/// <TYPE> production

	Node *type_specifier();

	/// <COMPOUNT_STMT> production

	Node *compound_stmt();

	/// <VAR_DECLARATION> production

	Node *var_declaration();

	/// <LOCAL_DECLARATIONS> production

	Node *local_declarations();

	/// <STATEMENT_LIST> production

	Node *statement_list();

	/// <STATEMENT> production

	Node *statement();

	/// <EXPRESSION_STMT> production

	Node *expression_stmt();

	/// <RETURN_STMT> production

	Node *return_stmt();

	/// <ASSIGN_EXPRESSION> production

	Node *assign_expression();

	/// <SIMPLE_EXPRESSION> production

	Node *simple_expression();

	/// <ADDOP> production

	Node *addop();

	/// <TERM> production

	Node *term();

	/// <MULOP> production

	Node *mulop();

	/// <FACTOR> production

	Node *factor();

protected:
	Node *_root;//< SyntaxTree root
	const TokenType *_current_token; //< Current token
	const TokenType *_token_head; //< Head of token list
	StringList::iterator _vars; //< Vars list
	std::unordered_set<std::string> _varstable;//< Declared vars
	const int* tokenlines; //< Lines list
};

#endif

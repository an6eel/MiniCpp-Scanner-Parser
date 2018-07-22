/**
	*  @file  SyntaxTree.cpp
	*  @author  Ángel García Malagón
	*  @version 1.0
	*
	* @brief File with implementation of SyntaxTree class
	*
	*/

#include "SyntaxTree.h"
#include <iostream>

using namespace std;

SyntaxTree::SyntaxTree(const TokenType *tokens, StringList::iterator vars,const int* lines){
	_root = NULL;
	tokenlines=lines;
	parse(tokens, vars);
}

SyntaxTree::~SyntaxTree(){
	if(_root != NULL){
		delete _root;
	}
}

bool SyntaxTree::parse(const TokenType *tokens, StringList::iterator vars){
	if(_root){
		delete _root;
	}

	_current_token = _token_head = tokens;
	_vars = vars;
	_root = program();

	if(_root)
		return true;
	else
		return false;
}

void SyntaxTree::print(){
	print(_root, 0);
}

void SyntaxTree::print(Node *parent, int depth){
	if(parent == NULL){
		return;
	}

	for(int i = 0; i < depth; i++){
		cout << "  ";
	}

	if(parent->type() == ID || parent->type() == NUM){
		cout << tokenString(parent->type()) << ": " << parent->value << endl;
	}else{
		string msg=parent->type() == EMPTY ? "-+" : tokenString(parent->type());
		cout << msg << endl;
	}

	if(parent->child != NULL){
		print(parent->child, depth + 1);
	}

	if(parent->sibling != NULL){
		print(parent->sibling, depth);
	}
}

TokenType SyntaxTree::currentToken(){
	return *_current_token;
}

TokenType SyntaxTree::forwardToken(){
	TokenType token = *_current_token;
	nextToken();
	return token;
}

void SyntaxTree::nextToken(){
	if(*_current_token != ENDFILE){
		_current_token++;
		tokenlines++;
	}
}

void SyntaxTree::prevToken(){
	_current_token--;
	tokenlines--;
}

const TokenType *SyntaxTree::tokenPosition(){
	return _current_token;
}

void SyntaxTree::setTokenPosition(const TokenType *token){
	tokenlines=tokenlines+(token-_current_token);
	_current_token = token;
}

bool SyntaxTree::matchToken(TokenType token){
	TokenType cur = currentToken();
	if(cur == token){
		nextToken();
		return true;
	}else{
	  cout << "Line "<< *tokenlines << "-> Syntax Error: Unexpected token " << tokenString(cur) << ". " << tokenString(token) << " expected." << endl;
		exit(-1);
	}
}

SyntaxTree::Node::Node(TokenType type){
	_type = type;

	child = NULL;
	sibling = NULL;
}

SyntaxTree::Node::Node(Node *node){
	_type = EMPTY;
	child = node;
	sibling = NULL;
}

SyntaxTree::Node::~Node(){
	if(child){
		delete child;
	}

	if(sibling){
		delete sibling;
	}
}

void SyntaxTree::Node::setType(TokenType type){
	_type = type;
}

TokenType SyntaxTree::Node::type() const{
	return _type;
}

SyntaxTree::Node *SyntaxTree::Node::setChild(TokenType type){
	if(child){
		delete child;
	}

	child = new Node(type);
	return child;
}

SyntaxTree::Node *SyntaxTree::Node::setSibling(TokenType type){
	if(sibling){
		delete sibling;
	}

	sibling = new Node(type);
	return sibling;
}

std::string SyntaxTree::getVar(){
	std::string cur = *_vars;
	_vars++;
	return cur;
}

SyntaxTree::Node *SyntaxTree::program(){
	return declaration();
}

SyntaxTree::Node *SyntaxTree::declaration(){
	Node *parent = type_specifier();
	if(parent){
		matchToken(ID);
		Node *dec = parent->setChild(ID);
		dec->value = getVar();

		TokenType token = forwardToken();

		if(token == LEFT_PARENTHESE){
			matchToken(RIGHT_PARENTHESE);
			dec->sibling = compound_stmt();
		}else{
			delete parent;
			return NULL;
		}
	}

	return parent;
}

SyntaxTree::Node *SyntaxTree::type_specifier(){
	TokenType token = forwardToken();

	if(token == INT || token == VOID){
		return new Node(token);
	}


	prevToken();
	return NULL;
}


SyntaxTree::Node *SyntaxTree::compound_stmt(){
	if(matchToken(LEFT_BRACE)){
		Node *local_dec = local_declarations();
		Node *stmt_list = statement_list();

		Node *parent = new Node;
		if(local_dec){
			parent->child = local_dec;
			if(stmt_list){
				parent->child->sibling = stmt_list;
			}
		}else if(stmt_list){
			parent->child = stmt_list;
		}

		if(matchToken(RIGHT_BRACE)){
			return parent;
		}

		delete parent;
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::var_declaration(){
	Node *parent = type_specifier();
	if(parent){
		if(forwardToken() == ID){
			Node *child = parent->setChild(ID);
			child->value = getVar();
			if(_varstable.find(child->value)==_varstable.end())
				_varstable.insert(child->value);
			else{
				string err="error: redeclaration of variable "+child->value;
				error(err);
			}
			if(matchToken(SEMICOLON)){
				return parent;
			}
		}
		delete parent;
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::local_declarations(){
	Node *head = var_declaration();

	if(head){
		Node *vard = head;
		while(vard){
			vard->sibling = var_declaration();
			vard = vard->sibling;
		}

		Node *parent = new Node;
		parent->child = head;
		return parent;
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::statement_list(){
	Node *head = statement();
	if(head){
		Node *stmt = head;
		while(stmt){
			stmt->sibling = statement();
			stmt = stmt->sibling;
		}

		Node *list = new Node;
		list->child = head;
		return list;
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::statement(){
	Node *stmt = NULL;
	switch(currentToken()){
		case ID:
			stmt = expression_stmt();
			break;
		case RETURN:
			stmt = return_stmt();
			break;
	}

	return stmt;
}

SyntaxTree::Node *SyntaxTree::expression_stmt(){
	const TokenType *opos = tokenPosition();

	Node *exp = assign_expression();
	if(exp){
		if(matchToken(SEMICOLON)){
			return exp;
		}
		delete exp;
	}


	setTokenPosition(opos);
	return NULL;
}


SyntaxTree::Node *SyntaxTree::return_stmt(){
	if(matchToken(RETURN)){
		Node *stmt = new Node(RETURN);
		Node *exp = simple_expression();
		if(exp){
			stmt->child = exp;
			if(matchToken(SEMICOLON)){
				return stmt;
			}
		}

		delete stmt;
	}

	return NULL;
}

SyntaxTree::Node* SyntaxTree::assign_expression(){
	Node *exp = factor();

	if(exp == NULL){
		delete exp;
	}else{
			Node *var = exp;
			exp = new Node(ASSIGN);
			exp->child = var;

			if(exp->child){
				Node *part = exp->child;
				if(matchToken(ASSIGN)){
					part->sibling = simple_expression();

					if(part->sibling){
						return exp;
					}
					else{
						string err="Syntax Error: Expected argument after  " + tokenString(exp->type());
						error(err);
					}
				}
			}
		}
}


SyntaxTree::Node *SyntaxTree::simple_expression(){
	const TokenType *opos = tokenPosition();

	Node *left = term();
	if(left){
		Node *op = addop();
		while(op){
			left->sibling = term();
			if(!left->sibling){
				string err="Syntax Error: Expected argument after operator " + tokenString(op->type());
				error(err);
			}
			op->child = left;

			left = op;
			op = addop();
		}

		return left;
	}

	setTokenPosition(opos);
	return NULL;
}


SyntaxTree::Node *SyntaxTree::addop(){
	TokenType token = currentToken();

	if(token == PLUS || token == MINUS){
		nextToken();
		return new Node(token);
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::term(){
	const TokenType *opos = tokenPosition();

	Node *left = factor();
	if(left){
		Node *op = mulop();
		while(op){
			left->sibling = factor();
			if(!left->sibling){
				string err="Syntax Error: Expected argument after operator " + tokenString(op->type());
				error(err);
			}
			op->child = left;

			left = op;
			op = mulop();
		}

		return left;
	}

	setTokenPosition(opos);
	return NULL;
}

SyntaxTree::Node *SyntaxTree::mulop(){
	TokenType token = currentToken();
	if(token == MULTI || token == DIV){
		nextToken();
		return new Node(token);
	}

	return NULL;
}

SyntaxTree::Node *SyntaxTree::factor(){
	const TokenType *opos = tokenPosition();

	TokenType token = forwardToken();
	Node *head = NULL, *part = NULL;
	TokenType next = EMPTY;
	switch(token){
		case LEFT_PARENTHESE:
			head = simple_expression();
			if(head && matchToken(RIGHT_PARENTHESE)){
				return head;
			}
			break;
		case ID:
			prevToken();

			if(matchToken(ID)){
				Node *id_node = new Node(ID);
				id_node->value = getVar();
				if(_varstable.find(id_node->value)==_varstable.end()){
					string err="Syntax Error: Variable "+ id_node->value + " was not declared";
					error(err);
				}

				return id_node;
			}
			return NULL;
			break;
		case NUM:
			Node *node = new Node(NUM);
			node->value = getVar();
			return node;
			break;
	}

	if(head){
		delete head;
	}
	setTokenPosition(opos);
	return NULL;
}

void SyntaxTree::error(string message){
	int line=*tokenlines;
	string lin="Line " + to_string(line)+ "-> ";
	lin+=message;
	cerr << lin << endl;

	exit(-1);
}

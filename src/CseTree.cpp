/**
	*  @file  CseTree.cpp
	*  @author  Ángel García Malagón
	*  @version 1.0
	*
	* @brief File with implementation of CseTree class
	*
	*/

#include "CseTree.h"
#include <string>
#include <iostream>
#include "structs.h"

using namespace std;

CseTree::CseTree(const TokenType *tokens, StringList::iterator vars,const int* lines):SyntaxTree(tokens,vars,lines),subexpr(0){

}

SyntaxTree::Node* CseTree::findType(SyntaxTree::Node*& node,TokenType t) {
  if(node == NULL){
		return NULL;
	}

	if(node->child!=NULL and node->child->type()==t){
		return node;
	}

  SyntaxTree::Node* n1=NULL;
  SyntaxTree::Node* n2=NULL;
	if(node->child != NULL){
		n1=findType(node->child,t);
	}

	if(node->sibling != NULL){
		n2=findType(node->sibling,t);
	}

  if(n1!=NULL)
    return n1;
  else if(n2!=NULL)
    return n2;
  else return NULL;
}

SyntaxTree::Node* CseTree::getAssigns() {
  SyntaxTree::Node* ass=findType(_root,ASSIGN);
  return ass;
}

SyntaxTree::Node* CseTree::getDeclarations(){
  SyntaxTree::Node* ass=findType(_root->child,INT);
  ass=ass->child;
  return ass;
}

void CseTree::print(){
  SyntaxTree::print();
}

bool CseTree::isOperator(SyntaxTree::Node*& node) const {
  return node !=NULL and (node->type()==MINUS or node->type()==PLUS or node->type()==MULTI or node->type()==DIV);
}

bool CseTree::BinaryExpr(SyntaxTree::Node*& node) const{
  return node !=NULL and isOperator(node) and !isOperator(node->child) and !isOperator(node->child->sibling);
}

bool CseTree::SameBExpr(SyntaxTree::Node*& n1,SyntaxTree::Node*& n2) const {
  bool cond= n1->type() == n2->type() and
             ((n1->child->value==n2->child->value and n1->child->sibling->value == n2->child->sibling->value)
              or ( n1->type()!=DIV and n1->child->value==n2->child->sibling->value and n2->child->value == n1->child->sibling->value));

  return cond;

}

bool CseTree::areIdentical(SyntaxTree::Node*& root1,SyntaxTree::Node*&  root2) const {
    if (root1 == NULL && root2 == NULL)
        return true;

    if (root1 == NULL || root2 == NULL)
        return false;


    if((isOperator(root1) and !isOperator(root2))or (!isOperator(root1) and isOperator(root2))){

      return false;
    }

    if(BinaryExpr(root1) and BinaryExpr(root2)){
      if(SameBExpr(root1,root2))
        return true;
      else return false;
    }


    return (!isOperator(root1) and !isOperator(root2) and root1->value==root2->value) or
           ( (isOperator(root1) and isOperator(root2) and root1->type()==root2->type() ) and
            (root1->child!=NULL and root2->child!=NULL and
            areIdentical(root1->child, root2->child) and
            areIdentical(root1->child->sibling, root2->child->sibling))) ;
}

bool CseTree::isSubtree(SyntaxTree::Node*& T,SyntaxTree::Node*& S,std::string mod){

    if (S == NULL)
        return true;

    if (T == NULL)
        return false;

    if (S->child !=NULL and areIdentical(T, S)){

      SyntaxTree::Node* sib=T->sibling;
      std::string n;
      if(mod==""){
        n="tmp"+to_string(subexpr);
        cs_found[n]=copyNode(T);
      }

      T=new Node(ID);
      T->value=mod=="" ? n :mod;
      T->sibling=sib;

      return true;
    }
    else if (BinaryExpr(T) and BinaryExpr(S))
      return false;

    if(T->child==NULL)
      return false;

    bool left=isSubtree(T->child, S,mod);
    bool right=isSubtree(T->child->sibling, S,mod);
    return  left || right;
}

std::string CseTree::ExprToString(SyntaxTree::Node*& node) const {
  std::string st="";
  if(node!=NULL){
    if(node->child!=NULL)
     st=st+ExprToString(node->child);

    if(isOperator(node)or node->type()==ASSIGN)
      st=st+operatorString(node->type());
    else
      st=st+ node->value;
    if(node->child!=NULL and node->child->sibling!=NULL)
      st=st+ExprToString(node->child->sibling) ;

  }

  return st;
}

SyntaxTree::Node* CseTree::copyNode(SyntaxTree::Node*& n) const{
  SyntaxTree::Node* n1=new Node(n->type());
  n1->value=n->value;
  if(n->sibling!=NULL)
    n1->sibling=copyNode(n->sibling);
  if(n->child!=NULL)
    n1->child=copyNode(n->child);

  return n1;

}

void CseTree::find(SyntaxTree::Node*& n1,SyntaxTree::Node*& tree){

  bool bal=n1->child!=NULL and (n1->type()==PLUS or n1->type()==MINUS)
           and n1->type()==n1->child->type() and n1->child->sibling!=NULL
           and n1->child->sibling->type()==ID and n1->child->child!=NULL
           and n1->child->child->sibling!=NULL and n1->child->child->sibling->type()==ID;

  if(isSubtree(tree,n1)){
    SyntaxTree::Node* sib=n1->sibling;
    n1=new Node(ID);
    std::string n="tmp"+to_string(subexpr);
    subexpr++;
    n1->value=n;
    n1->sibling=sib;
  }
  else{

    if(bal){
      SyntaxTree::Node* adop=new Node(n1->type());
      adop->child=copyNode(n1->child->child->sibling);
      adop->child->sibling=copyNode(n1->child->sibling);

      if(isSubtree(tree,adop)){
        n1->child=n1->child->child;
        n1->child->sibling=new Node(ID);
        std::string n="tmp"+to_string(subexpr);
        subexpr++;
        n1->child->sibling->value=n;
      }

      for(auto it=cs_found.begin();it!=cs_found.end();++it)
        if(areIdentical(adop,it->second)){
          n1->child=n1->child->child;
          n1->child->sibling=new Node(ID);
          n1->child->sibling->value=it->first;
        }
    }
    if(n1!=NULL){
      if(n1->child!=NULL){
        find(n1->child,tree);

        if(n1->child->sibling!=NULL){
          find(n1->child->sibling,tree);
        }
      }
    }
  }

}

void CseTree::ApplyCSE(){
  SyntaxTree::Node* ass=getAssigns();
  ass=ass->child;
  SyntaxTree::Node* tolast=ass;

  while(ass!=NULL){
    SyntaxTree::Node* aux=ass->sibling;

    for(auto it=cs_found.begin();it!=cs_found.end();++it)
      isSubtree(ass->child->sibling,it->second,it->first);

    if(aux==NULL) // I have to compare the last assign with other one
      find(ass->child->sibling,tolast->child->sibling);

    while(aux!=NULL){
      	find(ass->child->sibling,aux->child->sibling);
        aux=aux->sibling;
    }

    ass=ass->sibling;
  }

  if(cs_found.size()){
    SyntaxTree::Node* decl=getDeclarations();
    while(decl->sibling !=NULL){
      decl=decl->sibling;
    }

    for(auto it=cs_found.begin();it!=cs_found.end();++it){
      SyntaxTree::Node* ndecl=new Node(INT);
      SyntaxTree::Node* declint=new Node(ID);
      declint->value=it->first;
      ndecl->child=declint;
      decl->sibling=ndecl;
      decl=decl->sibling;

    }


    // New Operations
    SyntaxTree::Node* assg=getAssigns();
    SyntaxTree::Node* backup=assg->child;
    auto it=cs_found.begin();
    SyntaxTree::Node* nassig=new Node(ASSIGN);
    nassig->child=new Node(ID);
    nassig->child->value=it->first;
    nassig->child->sibling=it->second;
    assg->child=nassig;
    ++it;
    for(;it!=cs_found.end();++it){
      SyntaxTree::Node* aux=new Node(ASSIGN);
      aux->child=new Node(ID);
      aux->child->value=it->first;
      aux->child->sibling=it->second;
      nassig->sibling=aux;
      nassig=nassig->sibling;
    }
    nassig->sibling=backup;
  }


}

void CseTree::printCode(std::ostream& os){
  for(int i=0;i<2;++i)
    os << endl;
  if(*SyntaxTree::_token_head==INT)
    os << "int main(void){" << endl;
  else
    os << "void main(void){" << endl;
  SyntaxTree::Node* decl=getDeclarations();
  while(decl!=NULL){
    os << "\tint " << decl->child->value << ";" << endl;
    decl=decl->sibling;
  }

  for(int i=0;i<1;++i)
    os << endl;

  decl=getAssigns();
  decl=decl->child;
  while(decl!=NULL){
    os << "\t" << ExprToString(decl) << ";" << endl;
    decl=decl->sibling;
  }

  os << endl;
  os << "}"<<endl<<endl;
}

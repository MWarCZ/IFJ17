//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/ast.h                    //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef AST_H
#define AST_H

#include "token.h"
#include "list.h"

typedef enum {
  AST_NA = 0,
  AST_Program,
  AST_ListDecDef,
  AST_FunctionHead,
  AST_ListParam,
  AST_Param,
  AST_NextParam,
  AST_DataType,
  AST_FunctionEnd,
  AST_ScopeDef,
  AST_ScopeHead,
  AST_ScopeEnd,
  AST_ScopeAfter,
  AST_FunctionBody,
  AST_ListVarDef,
  AST_VarDefAssigment,
  AST_ListCommand,
  AST_Command,
  AST_ListExpression,
  AST_Condition,
  AST_Assignment,
  AST_ListInParam,
  AST_InParam,
  AST_NextInParam,
  AST_Term,
  AST_Expression
} TATSNodeType;

typedef struct TATSNode {
  TATSNodeType type;
  TToken *token1;
  TToken *token2;
  TToken *token3;
  TList *listPostFix;
  struct TATSNode *node1;
  struct TATSNode *node2;
  struct TATSNode *node3;
  struct TATSNode *node4;
} TATSNode;

TATSNode* InitASTNode(TATSNodeType type);
void DestroyASTNode(TATSNode *node);
void DestroyASTNodeSafely(TATSNode **node);

void PrintASTNodeType(TATSNodeType type) ;

#endif

// Soubor: src/ast.h

//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/generator.h              //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"


void GeneratorStart(TATSNode **nodeATS);

void Generator_Expression(TList *listPostFix);
void Generator_StringExpression(TList *listPostFix);

void Generator_Program(TATSNode **nodeATS);
void Generator_ListDecDef(TATSNode **nodeAST);
void Generator_FunctionHead(TATSNode **nodeAST);
void Generator_ListParam(TATSNode **nodeAST);
void Generator_Param(TATSNode **nodeAST);
void Generator_NextParam(TATSNode **nodeAST);
void Generator_DataType(TATSNode **nodeAST);
void Generator_FunctionEnd(TATSNode **nodeAST);
void Generator_ScopeDef(TATSNode **nodeAST);
void Generator_ScopeHead(TATSNode **nodeAST);
void Generator_ScopeEnd(TATSNode **nodeAST);
void Generator_ScopeAfter(TATSNode **nodeAST);
void Generator_FunctionBody(TATSNode **nodeAST);
void Generator_ListVarDef(TATSNode **nodeAST);
void Generator_VarDefAssigment(TATSNode **nodeAST);
void Generator_ListCommand(TATSNode **nodeAST);
void Generator_Command(TATSNode **nodeAST);
void Generator_ListExpression(TATSNode **nodeAST);
void Generator_Condition(TATSNode **nodeAST);
void Generator_Assignment(TATSNode **nodeAST);
void Generator_ListInParam(TATSNode **nodeAST);
void Generator_InParam(TATSNode **nodeAST);
void Generator_NextInParam(TATSNode **nodeAST);
void Generator_Term(TATSNode **nodeAST);

void Generator_Concat(TToken *token,TList *listPostFix);
void Generator_Strlen(TToken *token,TList *listPostFix);
void Generator_Getchar();
void Generator_Setchar();
void Generator_Type();
void Generator_EscapeSequence();

#endif

// Soubor: src/generator.h


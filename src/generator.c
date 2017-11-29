//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/generator.c              //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////


#ifndef GENERATOR_C
#define GENERATOR_C

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "generator.h"
#include "ast.h"
#include "token.h"
#include "list.h"

/*
  funkce s nazvem 'xyz': f%xyz
  promena/parametr s nazvem 'xyz': p%xyz

 */

void GPrint_ProgramHead() {
  printf(".ifjcode17\njump SCOPE\n");
}

// Scope
void GPrint_ScopeHead() {
  printf("label SCOPE\ncreateframe\npushframe\n");
}
void GPrint_ScopeFoot() {
  printf("popframe\n");
}
//-----------

// Function
void GPrint_FunctionHead(char* name) {
  printf("label f%%%s\npushframe\n",name);
}
void GPrint_FunctionParam(char* name) {
  printf("defvar LF@p%%%s\npops LF@p%%%s\n", name, name);
}
void GPrint_FunctionFoot() {
  printf("popframe\nreturn\n");
}
//-----------

void GPrint_LocalVariableCreateStart(char* name) {
  printf("defvar LF@p%%%s\n", name);
}

void GPrint_LocalVariableCreateEnd(char* name) {
  printf("pops LF@p%%%s\n", name);
}

// Call Function
void GPrint_FunctionCallStart(TTokenType returnType) {
  printf("createframe\ndefvar TF@%%retval\n");
  printf("mov TF@%%retval ");
  switch(returnType) {
    case TK_NUM_INTEGER:
      printf("int@0\n");
      break;
    case TK_NUM_DOUBLE:
      printf("double@0.0\n");
      break;
    case TK_NUM_STRING:
      printf("string@\n");
      break;
    default:
      printf("bool@false\n");
      break;
  }

}
void GPrint_FunctionCallInParam() {

}
void GPrint_FunctionCallEnd(char* name) {
  printf("call %s\n", name);
}
//--------

void GPrint_PushLocalVariable(char* name) {
  printf("pushs LF@p%%%s\n", name );
}
void GPrint_PushInt(int value) {
  printf("pushs int@%d\n", value );
}
void GPrint_PushDouble(double value) {
  printf("pushs double@%g\n", value );
}
void GPrint_PushString(char* value) {
  printf("pushs string@%s\n", value );
}

void GPrint_ConverseResult(TTokenType varType){
  if(varType == TK_NUM_DOUBLE){
    printf("int2floats\n");
  }
  else{
    printf("float2ints\n");
  }
}

//-------------
//
//-------------

void GeneratorStart(TATSNode **nodeAST) {
  Generator_Program(nodeAST);
  printf("\n");
}

void Generator_Program(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_Program\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  GPrint_ProgramHead();
  Generator_ListDecDef( &((*nodeAST)->node1) );
  Generator_ScopeDef( &((*nodeAST)->node2) );
}

void Generator_ListDecDef(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_ListDecDef\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  printf("\n");
  if( (*nodeAST)->token1 != NULL && (*nodeAST)->token1->type == TK_DECLARE ) {
    Generator_ListDecDef( &((*nodeAST)->node2) );
    //node1->FunctionHead????
  }
  else {
    Generator_FunctionHead( &((*nodeAST)->node1) );
    Generator_FunctionBody( &((*nodeAST)->node2) );
    Generator_FunctionEnd( &((*nodeAST)->node3) );
    Generator_ListDecDef( &((*nodeAST)->node4) );
  }

}
void Generator_FunctionHead(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_FunctionHead\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  GPrint_FunctionHead( (*nodeAST)->token1->string );
  Generator_ListParam( &((*nodeAST)->node1) );
  //node2->DataType
}
void Generator_ListParam(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_ListParam\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  if( (*nodeAST)->node1 != NULL ) {
    Generator_Param( &((*nodeAST)->node1) );
    Generator_NextParam( &((*nodeAST)->node2) );
  }

}
void Generator_Param(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_Param\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  GPrint_FunctionParam( (*nodeAST)->token1->string );
  //node1->DataType
  //token2 ->NULL|TK_NUM_* ??
}
void Generator_NextParam(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_NextParam\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  if( (*nodeAST)->token1 != NULL ) {
    Generator_Param( &((*nodeAST)->node1) );
    Generator_NextParam( &((*nodeAST)->node2) );
  }
  
}
void Generator_DataType(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_DataType\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  //token1 -> Int,Double,String
}
void Generator_FunctionEnd(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_FunctionEnd\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  //GPrint_FunctionFoot();
}
void Generator_ScopeDef(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_ScopeDef\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  Generator_ScopeHead( &((*nodeAST)->node1) );
  Generator_FunctionBody( &((*nodeAST)->node2) );
  printf("label end%%SCOPE\n");
  Generator_ScopeEnd( &((*nodeAST)->node3) );

}
void Generator_ScopeHead(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_ScopeHead\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  GPrint_ScopeHead();
}
void Generator_ScopeEnd(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_ScopeEnd\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );

  GPrint_ScopeFoot();
}
void Generator_ScopeAfter(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_ScopeAfter\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  

}
void Generator_FunctionBody(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_FunctionBody\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  Generator_ListVarDef( &((*nodeAST)->node1) );
  Generator_ListCommand( &((*nodeAST)->node2) );

}
void Generator_ListVarDef(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_ListVarDef\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  printf("\n");
  
  if( (*nodeAST)->token1 != NULL ) {
    GPrint_LocalVariableCreateStart( (*nodeAST)->token1->string );

    //node1->DataType

    Generator_VarDefAssigment( &((*nodeAST)->node2) );

    GPrint_LocalVariableCreateEnd( (*nodeAST)->token1->string );

    Generator_ListVarDef( &((*nodeAST)->node3) );
  }

}

void Generator_VarDefAssigment(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_VarDefAssigment\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  if( (*nodeAST)->listPostFix != NULL ) {
    if( (*nodeAST)->token2->type == TK_NUM_STRING ) {
      Generator_StringExpression( (*nodeAST)->listPostFix );
    }
    else {
      Generator_Expression( (*nodeAST)->listPostFix );
    }
  }
  else {
    switch( (*nodeAST)->token1->type ) {
      case TK_NUM_INTEGER:
        GPrint_PushInt( 0 );
        break;
      case TK_NUM_DOUBLE:
        GPrint_PushDouble( 0.0 );
        break;
      case TK_NUM_STRING:
        GPrint_PushString( "" );
        break;
      default:
        break;
    }
  }

//   if( listPostFix <> NULL )
//     listPostFix > list tokenu v postfix formatu
//     token2 > TK_NUM_* | TK_TRUE // Urcuje datovy typ vysledku vyrazu
//     token1 > TK_NUM_* | TK_TRUE // Urcuje datovy typ ktery ma byt 
// // prirazen do promnene
  
}

void Generator_Expression(TList *listPostFix) {
  TListData data;
  int i = 0;
  //printf("generatorExpr\n");
  while( ListGet(listPostFix, i, &data ) ) {
    switch( ((TToken*)data.pointer)->type ) {
      case TK_ID:
        GPrint_PushLocalVariable( ((TToken*)data.pointer)->string );
        break;
      case TK_NUM_INTEGER:
        GPrint_PushInt( ((TToken*)data.pointer)->data.integerValue );
        break;
      case TK_NUM_DOUBLE:
        GPrint_PushDouble( ((TToken*)data.pointer)->data.doubleValue );
        break;
      case TK_NUM_STRING:
        GPrint_PushString( ((TToken*)data.pointer)->string );
        break;
      case TK_TRUE:
      case TK_FALSE:
        printf("NA\n");
        break;
      case TK_PLUS:
        printf("adds\n");
        break;
      case TK_MINUS:
        printf("subs\n");
        break;
      case TK_MUL:
        printf("muls\n");
        break;
      case TK_DIV:
        printf("divs\n");
        break;
      case TK_DIV_INT:
        printf("NA\n");
        break;
      case TK_MOD:
        printf("NA\n");
        break;
      case TK_INT2FLOAT:
        printf("int2floats\n");
        break;
      case TK_FLOAT2INT:
        printf("float2ints\n");
        break;
      case TK_LESS:
        printf("lts\n");
        break;
      case TK_GREATER:
        printf("gts\n");
        break;
      case TK_EQUAL:
        printf("eqs\n");
        break;
      case TK_NOT_EQUAL:
        printf("eqs\nnots\n");
        break;
      default:
        printf("NA err\n");
        break;
    }
    i++;
  }

}

void Generator_StringExpression(TList *listPostFix) {
  TListData data;
  int i = 0;
  static unsigned int counterTmpVar = 0;
  unsigned int localCounter = counterTmpVar;
  counterTmpVar++;

  while( ListGet(listPostFix, i, &data ) ) {
    switch( ((TToken*)data.pointer)->type ) {
      case TK_ID:
        GPrint_PushLocalVariable( ((TToken*)data.pointer)->string );
        break;
      case TK_NUM_STRING:
        GPrint_PushString( ((TToken*)data.pointer)->string );
        break;
      case TK_PLUS:
        printf("defvar LF@tmp1%%%d\n",localCounter);
        printf("defvar LF@tmp2%%%d\n",localCounter);
        printf("pops LF@tmp1%%%d\n",localCounter);
        printf("pops LF@tmp2%%%d\n",localCounter);
        printf("concat LF@tmp1%%%d LF@tmp1%%%d LF@tmp2%%%d\n",localCounter, localCounter, localCounter);
        printf("pushs LF@tmp1%%%d\n",localCounter);
        localCounter = counterTmpVar;
        counterTmpVar++;
        break;
      case TK_LESS:
        printf("lts\n");
        break;
      case TK_GREATER:
        printf("gts\n");
        break;
      case TK_EQUAL:
        printf("eqs\n");
        break;
      case TK_NOT_EQUAL:
        printf("eqs\nnots\n");
        break;
      default:
        printf("NA err\n");
        break;
    }
    i++;
  }
}

void Generator_ListCommand(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_ListCommand\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );

  if( (*nodeAST)->node1 != NULL ) {
    Generator_Command( &((*nodeAST)->node1) );
    Generator_ListCommand( &((*nodeAST)->node2) );
  }
}
void Generator_Command(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_Command\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );

  printf("\n");
  static unsigned int counter = 0;
  unsigned int localCounter = counter;
  counter++;
  
  if( (*nodeAST)->token1->type == TK_DO ) { // label: start_do%0 , end_do%0 
    printf("label start_do%%%d\n", localCounter ); 
    Generator_Condition( &((*nodeAST)->node1) );
    printf("pushs bool@false\njumpifeqs end_do%%%d\n", localCounter);
    Generator_ListCommand( &((*nodeAST)->node2) );
    printf("jump start_do%%%d\nlabel end_do%%%d\n", localCounter, localCounter);
    // token1 > TK_DO
    // node1 > Condition
    // node2 > ListCommand

  }
  else if( (*nodeAST)->token1->type == TK_IF ) { // label: else_if%0 , end_if%0
    Generator_Condition( &((*nodeAST)->node1) );
    printf("pushs bool@false\njumpifeqs else_if%%%d\n", localCounter);
    Generator_ListCommand( &((*nodeAST)->node2) );
    printf("jump end_if%%%d\nlabel else_if%%%d\n", localCounter, localCounter);
    Generator_ListCommand( &((*nodeAST)->node3) );
    printf("label end_if%%%d\n", localCounter);
    // token1 > TK_IF
    // node1 > Condition
    // node2 > ListCommand
    // node3 > ListCommand

  }
  else if( (*nodeAST)->token1->type == TK_ID ) {
    Generator_Assignment( &((*nodeAST)->node1) );
    if((*nodeAST)->node1->token2->type != (*nodeAST)->token1->type){
      GPrint_ConverseResult((*nodeAST)->token2->type);
    }
    GPrint_LocalVariableCreateEnd( (*nodeAST)->token1->string );
    // token1 > TK_ID
    // node1 > Assignment  
    
  }
  else if( (*nodeAST)->token1->type == TK_RETURN ) {
    // // return 
    // token1 > TK_RETURN
    // listPostFix > list tokenu v postfix formatu
    // token2 > TK_NUM_* | TK_TRUE // Urcuje datovy typ vysledku vyrazu
    if( (*nodeAST)->token2->type == TK_NUM_STRING ) {
      Generator_StringExpression( (*nodeAST)->listPostFix );
    }
    else {
      Generator_Expression( (*nodeAST)->listPostFix );
    }
    GPrint_FunctionFoot();

  }
  else if( (*nodeAST)->token1->type == TK_PRINT ) {
    // // print 
    // token1 >TK_PRINT
    // node1 > ListExpression
    Generator_ListExpression( &((*nodeAST)->node1) );
  }
  else if( (*nodeAST)->token1->type == TK_INPUT ) {/////////////
    // // input id
    // token1 > TK_INPUT
    // token2 > TK_ID
    //token3 > DATA_TYPE
    if((*nodeAST)->token2->type == TK_ID){
      if((*nodeAST)->token3->type == TK_NUM_STRING){
        printf("read LF@%%%s string\n",(*nodeAST)->token2->string);
      }
      else if((*nodeAST)->token3->type == TK_NUM_INTEGER){
        printf("read LF@%%%s int\n",(*nodeAST)->token2->string);
      }
      else if((*nodeAST)->token3->type == TK_NUM_DOUBLE){
        printf("read LF@%%%s float\n",(*nodeAST)->token2->string);
      }
    }
  }
}
void Generator_ListExpression(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_ListExpression\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  static unsigned int ExprCounter = 0;
  unsigned int locCounter = ExprCounter;
  ExprCounter++;

  if((*nodeAST)->listPostFix != NULL){
    if((*nodeAST)->token2->type == TK_NUM_STRING){
      Generator_StringExpression((*nodeAST)->listPostFix);
    }
    else{
      Generator_Expression((*nodeAST)->listPostFix);
    }
    printf("defvar p_tmp%%%d\n",locCounter);
    printf("pops p_tmp%%%d\n",locCounter);
    printf("write p_tmp%%%d\n",locCounter);
    Generator_ListExpression(&((*nodeAST)->node1));
  }
}
void Generator_Condition(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_Condition\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );

  if( (*nodeAST)->listPostFix != NULL ) {
    Generator_Expression( (*nodeAST)->listPostFix );
  }
  
}
void Generator_Assignment(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_Assignment\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );

  if((*nodeAST)->token1 == NULL){
    if((*nodeAST)->token2->type == TK_STRING){
      Generator_StringExpression((*nodeAST)->listPostFix);
    }
    else{
      Generator_Expression((*nodeAST)->listPostFix);
    }
  }
  if((*nodeAST)->token1->type == TK_ID){
    //GPrint_FunctionCallStart((*nodeAST)->token2->type);
    //Generator_ListInParam( &((*nodeAST)->node1) );
  }
}
void Generator_ListInParam(TATSNode **nodeAST) {
  fprintf(stderr, ">> Generator_ListInParam\n");
  if( !nodeAST || !(*nodeAST) ) return;
  PrintASTNodeType( (*nodeAST)->type ); 

  if((*nodeAST)->node1 != NULL){
    Generator_InParam(&((*nodeAST)->node1));
    Generator_NextInParam(&((*nodeAST)->node2));
  }
}
void Generator_InParam(TATSNode **nodeAST) {
 //fprintf(stderr, ">> Generator_InParam\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );

  Generator_Term(&((*nodeAST)->node1));
}
void Generator_NextInParam(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_NextInParam\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );

  if((*nodeAST)->node1 != NULL){
    Generator_InParam(&((*nodeAST)->node1));
    Generator_NextInParam(&((*nodeAST)->node2));
  }
}
void Generator_Term(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_Term\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );  


}
//-----------------------------------

#endif

// Soubor: src/generator.c


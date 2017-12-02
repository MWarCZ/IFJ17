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
#include "string.h"

/*
  funkce s nazvem 'xyz': f%xyz
  promena/parametr s nazvem 'xyz': p%xyz

 */
/*Length funkce*/
void GPrint_Length(){
  printf("label f%%length\n");
  printf("pushframe\n");

  printf("defvar LF@p%%str\npops LF@p%%str\n");
  printf("defvar LF@p%%strlen\npushs int@0\npops LF@p%%strlen\n");
  printf("strlen LF@p%%strlen LF@p%%str\n");
  printf("move LF@%%retval LF@p%%strlen\n");

  printf("popframe\nreturn\n\n");
}

/*
void GPrint_SubStr(){
  printf("label f%%substr\n");
  printf("pushframe\n");

  printf("defvar LF@p%%StrEnd\npops LF@p%%StrEnd\n");
  printf("defvar LF@p%%StrBegin\npops LF@p%%StrBegin\n");
  printf("defvar LF@p%%Str\npops LF@%%Str\n");
  printf("defvar LF@p%%substrTmp\npushs bool@false\npops LF@p%%substrTmp\n");
  printf("defvar LF@p%%retString\npushs string@\npops LF@p%%retString\n");

  printf("pushs LF@p%%StrBegin\npushs int@0\nlts\n");
  printf("pushs LF@p%%StrBegin\npushs int@0\neqs\n");
  printf("ors\npops LF@p%%substrTmp");
  printf("pushs LF@p%%Str\npushs str@\neqs\npushs LF@p%%substrTmp\nors\n");

  printf("pushs bool@false\njumpifeqs else_if%%substr1\n");
    printf("clears\npushs LF@p%%StrEnd\npushs int@0\nlts\npushs LF@p%%StrEnd\n");
    printf("pushs LF@p%%Str\ncall f%%length\ngts\nors\n");
    printf("pushs bool@false\njumpifeqs else_if%%substr2\n");
      printf("");
    printf("jump end_if%%substr\nlabel else_if%%substr\n");
      printf("");
    printf("label end_if%%substr\n");
  printf("jump end_if%%substr\nlabel else_if%%substr\n");
    printf("move LF@%%retval LF@p%%retString\n");
  printf("label end_if%%substr\n");

}*/

void GPrint_Asc(){
  printf("label f%%asc\npushframe\ndefvar LF@p_tmp%%1\ndefvar LF@p_tmp%%2\ndefvar LF@p%%s\npops LF@p%%s\ndefvar LF@p%%i\npops LF@p%%i\ndefvar LF@p%%len\npushs int@0\npops LF@p%%len\ncreateframe\ndefvar TF@%%retval\nmove TF@%%retval int@0\npushs LF@p%%s\ncall f%%length\npushs TF@%%retval\npops LF@p%%len\npushs LF@p%%i\npushs int@0\nlts\npushs bool@false\njumpifeqs else_if%%1\npushs int@0\npops LF@%%retval\npopframe\nreturn\njump end_if%%1\nlabel else_if%%1\npushs LF@p%%i\npushs LF@p%%len\npops LF@p_tmp%%2\npops LF@p_tmp%%1\npushs LF@p_tmp%%1\npushs LF@p_tmp%%2\ngts\npushs LF@p_tmp%%1\npushs LF@p_tmp%%2\neqs\nors\npushs bool@false\njumpifeqs else_if%%3\npushs int@0\npops LF@%%retval\npopframe\nreturn\njump end_if%%3\nlabel else_if%%3\nstri2int LF@%%retval LF@p%%s LF@p%%i\nlabel end_if%%3\nlabel end_if%%1\npopframe\nreturn\n\n");

}

void GPrint_Chr(){
  printf("label f%%chr\npushframe\ndefvar LF@p_tmp%%1\ndefvar LF@p_tmp%%2\ndefvar LF@p%%i\npops LF@p%%i\npushs LF@p%%i\npushs int@0\nlts\npushs bool@false\njumpifeqs else_if%%0\npushs string@\npops LF@%%retval\npopframe\nreturn\njump end_if%%0\nlabel else_if%%0\npushs LF@p%%i\npushs int@255\ngts\npushs bool@false\njumpifeqs else_if%%2\npushs string@\npops LF@%%retval\npopframe\nreturn\njump end_if%%2\nlabel else_if%%2\nint2char LF@%%retval LF@p%%i\nlabel end_if%%2\nlabel end_if%%0\npopframe\nreturn\n\n");

}

void GPrint_PrintString(char* str){
  int len = strlen(str);
  for(int i=0; i<len; i++ ) {
    if( (str[i]>= 0 && str[i]<=32 )||(str[i]==92) ) {
      printf("\\%03d", str[i] );
    }
    else {
      printf("%c", str[i] );
    }
  }
}

void GPrint_ProgramHead() {
  printf(".ifjcode17\njump SCOPE\n\n");
  GPrint_Length();
  GPrint_Asc();
  GPrint_Chr();
}

// Scope
void GPrint_ScopeHead() {
  printf("label SCOPE\ncreateframe\npushframe\n");
  printf("defvar LF@p_tmp%%1\ndefvar LF@p_tmp%%2\n");
}
void GPrint_ScopeFoot() {
  printf("popframe\n");
}
//-----------

// Function
void GPrint_FunctionHead(char* name) {
  printf("label f%%%s\npushframe\n",name);
  printf("defvar LF@p_tmp%%1\ndefvar LF@p_tmp%%2\n");
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
  printf("move TF@%%retval ");
  switch(returnType) {
    case TK_NUM_INTEGER:
      printf("int@0\n");
      break;
    case TK_NUM_DOUBLE:
      printf("float@0.0\n");
      break;
    case TK_NUM_STRING:
      printf("string@\n");
      break;
    default:
      printf("bool@false\n");
      break;
  }

}
void GPrint_FunctionCallEnd(char* name) {
  printf("call f%%%s\n", name);
  printf("pushs TF@%%retval\n");
}
//--------

void GPrint_PushLocalVariable(char* name) {
  printf("pushs LF@p%%%s\n", name );
}
void GPrint_PushInt(int value) {
  printf("pushs int@%d\n", value );
}
void GPrint_PushDouble(double value) {
  printf("pushs float@%g\n", value );
}
void GPrint_PushString(char* value) {
  printf("pushs string@");
  GPrint_PrintString(value);
  printf("\n");
}

void GPrint_PopLocalVariable(char* name) {
  printf("pops LF@p%%%s\n", name );
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
  //Generator_DataType( &((*nodeAST)->node2) );
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
void Generator_Param(TATSNode **nodeAST) {///////////
  //fprintf(stderr, ">> Generator_Param\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  GPrint_FunctionParam( (*nodeAST)->token1->string );
  //Generator_DataType( &((*nodeAST)->node1) );
  //token2 ->NULL|TK_NUM_* ??
}
void Generator_NextParam(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_NextParam\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  if( (*nodeAST)->node1 != NULL ) {
    Generator_Param( &((*nodeAST)->node1) );
    Generator_NextParam( &((*nodeAST)->node2) );
  }
  
}
void Generator_DataType(TATSNode **nodeAST) { //////////
  //fprintf(stderr, ">> Generator_DataType\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  

}
void Generator_FunctionEnd(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_FunctionEnd\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );
  
  GPrint_FunctionFoot();
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
  if( !nodeAST || !(*nodeAST) ) return;
  
  GPrint_ScopeHead();
}
void Generator_ScopeEnd(TATSNode **nodeAST) {
  if( !nodeAST || !(*nodeAST) ) return;

  GPrint_ScopeFoot();
}
void Generator_ScopeAfter(TATSNode **nodeAST) {
  if( !nodeAST || !(*nodeAST) ) return;

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
    if( (*nodeAST)->token1->type == TK_NUM_INTEGER && (*nodeAST)->token2->type == TK_NUM_DOUBLE ) {
      printf("float2r2eints\n");
    }
    else if( (*nodeAST)->token1->type == TK_NUM_DOUBLE && (*nodeAST)->token2->type == TK_NUM_INTEGER ) {
      printf("int2floats\n");
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
      case TK_LESS_EQUAL:
        printf("pops LF@p_tmp%%2\n");
        printf("pops LF@p_tmp%%1\n");
        printf("pushs LF@p_tmp%%1\n");
        printf("pushs LF@p_tmp%%2\n");
        printf("lts\n");
        printf("pushs LF@p_tmp%%1\n");
        printf("pushs LF@p_tmp%%2\n");
        printf("eqs\n");
        printf("ors\n");
        break;
      case TK_GREATER_EQUAL:
        printf("pops LF@p_tmp%%2\n");
        printf("pops LF@p_tmp%%1\n");
        printf("pushs LF@p_tmp%%1\n");
        printf("pushs LF@p_tmp%%2\n");
        printf("gts\n");
        printf("pushs LF@p_tmp%%1\n");
        printf("pushs LF@p_tmp%%2\n");
        printf("eqs\n");
        printf("ors\n");
        break;
      case TK_DIV_INT:
        printf("pops LF@p_tmp%%2\n");
        printf("int2floats\n");
        printf("pushs LF@p_tmp%%2\n");
        printf("int2floats\n");
        printf("divs\n");
        printf("float2ints\n");
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
      case TK_LESS_EQUAL:
        printf("pops LF@p_tmp%%2\n");
        printf("pops LF@p_tmp%%1\n");
        printf("pushs LF@p_tmp%%1\n");
        printf("pushs LF@p_tmp%%2\n");
        printf("lts\n");
        printf("pushs LF@p_tmp%%1\n");
        printf("pushs LF@p_tmp%%2\n");
        printf("eqs\n");
        printf("ors\n");
        break;
      case TK_GREATER_EQUAL:
        printf("pops LF@p_tmp%%2\n");
        printf("pops LF@p_tmp%%1\n");
        printf("pushs LF@p_tmp%%1\n");
        printf("pushs LF@p_tmp%%2\n");
        printf("gts\n");
        printf("pushs LF@p_tmp%%1\n");
        printf("pushs LF@p_tmp%%2\n");
        printf("eqs\n");
        printf("ors\n");
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
  else if( (*nodeAST)->token1->type == TK_ID || (*nodeAST)->token1->type == TK_LENGTH || (*nodeAST)->token1->type == TK_SUBSTR || (*nodeAST)->token1->type == TK_ASC || (*nodeAST)->token1->type == TK_CHR ) {
    Generator_Assignment( &((*nodeAST)->node1) );

    if( (*nodeAST)->token2->type == TK_NUM_INTEGER && (*nodeAST)->node1->token2->type == TK_NUM_DOUBLE ) {
      printf("float2r2eints\n");
    }
    else if( (*nodeAST)->token2->type == TK_NUM_DOUBLE && (*nodeAST)->node1->token2->type == TK_NUM_INTEGER ) {
      printf("int2floats\n");
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

    if( (*nodeAST)->token3->type == TK_NUM_INTEGER && (*nodeAST)->token2->type == TK_NUM_DOUBLE ) {
      printf("float2r2eints\n");
    }
    else if( (*nodeAST)->token3->type == TK_NUM_DOUBLE && (*nodeAST)->token2->type == TK_NUM_INTEGER ) {
      printf("int2floats\n");
    }

    printf("pops LF@%%retval\n");

    GPrint_FunctionFoot();

  }
  else if( (*nodeAST)->token1->type == TK_PRINT ) {
    // // print 
    // token1 >TK_PRINT
    // node1 > ListExpression
    Generator_ListExpression( &((*nodeAST)->node1) );

  }
  else if( (*nodeAST)->token1->type == TK_INPUT ) {
    printf("write string@?\\032\n");
    printf("read LF@p%%%s ", (*nodeAST)->token2->string );
    switch( (*nodeAST)->token3->type ) {
      case TK_NUM_INTEGER: 
        printf("int\n");
        break;
      case TK_NUM_DOUBLE: 
        printf("float\n");
        break;
      case TK_NUM_STRING: 
        printf("string\n");
        break;
      default: 
        printf("string\n");
        break;
    }
  }
  else if( (*nodeAST)->token1->type == TK_LENGTH ){////////////

  }
  else if( (*nodeAST)->token1->type == TK_SUBSTR ){/////////////
   /* printf("createframe\n");
    printf("defvar TF@%%retstr\nmove TF@%%retstr string@\n");
    //parametry
    printf("call f%%substr\n");*/
  }
  else if( (*nodeAST)->token1->type == TK_ASC ){////////////

  }
  else if( (*nodeAST)->token1->type == TK_CHR ){///////////

  }
}
void Generator_ListExpression(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_ListExpression\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );

  if( (*nodeAST)->listPostFix != NULL ) {
    Generator_Expression( (*nodeAST)->listPostFix );
    printf("pops LF@p_tmp%%1\n" );
    printf("write LF@p_tmp%%1\n");
    Generator_ListExpression( &((*nodeAST)->node1) );
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
  else if( (*nodeAST)->token1->type == TK_ID || (*nodeAST)->token1->type == TK_LENGTH || (*nodeAST)->token1->type == TK_SUBSTR || (*nodeAST)->token1->type == TK_ASC || (*nodeAST)->token1->type == TK_CHR ) {
    //GPrint_FunctionCallStart((*nodeAST)->token2->type);
    GPrint_FunctionCallStart( (*nodeAST)->token2->type );
    Generator_ListInParam( &((*nodeAST)->node1) );
    GPrint_FunctionCallEnd( (*nodeAST)->token1->string );
  }
}
void Generator_ListInParam(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_ListInParam\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type ); 

  if((*nodeAST)->node1 != NULL){
    Generator_NextInParam(&((*nodeAST)->node2));
    Generator_InParam(&((*nodeAST)->node1));
  }
}
void Generator_InParam(TATSNode **nodeAST) {
 //fprintf(stderr, ">> Generator_InParam\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );

  Generator_Term( &((*nodeAST)->node1) );
  // token1 - typ pozadovaneho parametru funkce
  // token2 - typ vstupniho parametru funkce
  if( (*nodeAST)->token1->type == TK_NUM_INTEGER && (*nodeAST)->token2->type == TK_NUM_DOUBLE ) {
    printf("float2r2eints\n");
  }
  else if( (*nodeAST)->token1->type == TK_NUM_DOUBLE && (*nodeAST)->token2->type == TK_NUM_INTEGER ) {
    printf("int2floats\n");
  }

}
void Generator_NextInParam(TATSNode **nodeAST) {
  //fprintf(stderr, ">> Generator_NextInParam\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );

  if((*nodeAST)->node1 != NULL){
    Generator_NextInParam(&((*nodeAST)->node2));
    Generator_InParam(&((*nodeAST)->node1));
  }
}
void Generator_Term(TATSNode **nodeAST) {///////////
  //fprintf(stderr, ">> Generator_Term\n");
  if( !nodeAST || !(*nodeAST) ) return;
  //PrintASTNodeType( (*nodeAST)->type );  

  switch( (*nodeAST)->token1->type ) {
    case TK_ID:
      GPrint_PushLocalVariable( (*nodeAST)->token1->string );
      break;
    case TK_NUM_INTEGER:
      GPrint_PushInt( (*nodeAST)->token1->data.integerValue );
      break;
    case TK_NUM_DOUBLE:
      GPrint_PushDouble( (*nodeAST)->token1->data.doubleValue );
      break;
    case TK_NUM_STRING:
      GPrint_PushString( (*nodeAST)->token1->string );
      break;
    default:
      break;
  }

}
//-----------------------------------

#endif

// Soubor: src/generator.c


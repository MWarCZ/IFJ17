//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/parser2.c                //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef PARSER2_C
#define PARSER2_C

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "token.h"
#include "error.h"
#include "dstring.h"
#include "symtable.h"
#include "list.h"
#include "ast.h"
#include "parser2.h"
//#include "generator.h"

symtable_t *GlobalSymtable = NULL; /* TS */ // Globalni tabulka symbolu alias Tabulka fuknci


// Znici stary token a vrati novy
TToken* GetNextDestroyOldToken(TToken *tkn, int canGetEOL) {
  do {
    if( tkn )
      TokenDestroy( tkn );
    tkn = GetNextToken();
  } while( !canGetEOL &&  tkn->type == TK_EOL );
  return tkn;
}

int SyntaxStartParse() {
  SymtableInit(&GlobalSymtable); /* TS */
  TToken *tmpToken = NULL;
  TToken** tkn = &tmpToken;

  TATSNode *rootAST = NULL; // AST

  SyntaxInitBuildInFunction();

  //(*tkn) = GetNextToken();
  (*tkn) = GetNextDestroyOldToken( (*tkn),0 );

  if( Syntaxx_Program(tkn, &rootAST ) ) {
    //GeneratorStart(&rootAST); // GENERATOR
  }
  else if( ERR_EXIT_STATUS ) {
    // pokud exit status neni OK tak doslo k jinemu typu chyby jiz drive
  }
  else { // Doslko konkretne k syntakticke chybe
    // ERR_SYN
    PrintLineErr( (*tkn) );
    CallError(ERR_SYN);
  }


  TokenDestroy( (*tkn) );
  DestroyASTNodeSafely(&rootAST); // AST
  SymtableFree(GlobalSymtable);
  ClearScanner();

  return !ERR_EXIT_STATUS;
}

int Syntaxx_Program(TToken **tkn, TATSNode **nodeAST) {
  (*nodeAST) = InitASTNode(AST_Program); // AST
  switch( (*tkn)->type ) {
    case TK_DECLARE:
    case TK_FUNCTION:
    case TK_SCOPE:
    case TK_EOF:
      return Syntaxx_ListDecDef(tkn, &((*nodeAST)->node1) ) && Syntaxx_ScopeDef(tkn, &((*nodeAST)->node2)  );
      break;
    default:
      fprintf(stderr, "Program\n");
      return 0;
      break;
  }
}

int Syntaxx_ListDecDef(TToken **tkn, TATSNode **nodeAST) {
  symtable_elem_t *gel = NULL; /* TS */
  (*nodeAST) = InitASTNode(AST_ListDecDef); // AST
  switch( (*tkn)->type ) {
    case TK_DECLARE: /// declare
      (*nodeAST)->token1 = (*tkn); /// AST declare
      (*tkn) = GetNextToken();
      //(*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_FunctionHead(tkn, &((*nodeAST)->node1), &gel, 1 ) && Syntaxx_ListDecDef(tkn, &((*nodeAST)->node2)  );
      break;
    case TK_FUNCTION:
      return Syntaxx_FunctionHead(tkn, &((*nodeAST)->node1), &gel, 0 ) && Syntaxx_FunctionBody(tkn, &((*nodeAST)->node2), &gel ) && Syntaxx_FunctionEnd(tkn, &((*nodeAST)->node3), &gel, 0  ) && Syntaxx_ListDecDef(tkn, &((*nodeAST)->node4)  );
      break;
    case TK_SCOPE:
      return 1;
      break;
    default:
      fprintf(stderr, "ListDecDef\n");
      return 0;
      break;
  }
}

int Syntaxx_FunctionHead(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, int isDeclareNow) {
  int checkParamNow = 0; /* TS */
  (*nodeAST) = InitASTNode(AST_FunctionHead); // AST
  switch( (*tkn)->type ) {
    case TK_FUNCTION: /// function
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_ID ) { /// id
        return 0;
      }
      /* TS s*/
      (*gel) = SymtableFind(GlobalSymtable,  (*tkn)->string ); 
      if( (*gel) == NULL ) { // Pokud funkce neexistuje tak ji vytvor
        (*gel) = AddElemGlobal(GlobalSymtable,  (*tkn)->string );
        (*gel)->elemType = SYM_TYPE_FUNCTION;
        (*gel)->listParam = ListInit();
        checkParamNow = 0; // List parametru se nebude kontrolovat ale vytvaret
      }
      else {
        checkParamNow = 1; // List parametru se bude kontrolovat
      }
      if( isDeclareNow) {
        if( (*gel)->declared ) {
          // ERR_SEM
          PrintLineErr( (*tkn) );
          fprintf(stderr, "Funkce byla jiz drive deklarovana.\n");
          CallError(ERR_SEM);
          return 0;
        }
        (*gel)->declared = 1;
      }
      else {
        if( (*gel)->defined ) {
          // ERR_SEM
          PrintLineErr( (*tkn) );
          fprintf(stderr, "Funkce byla jiz drive definovana.\n");
          CallError(ERR_SEM);
          return 0;
        }
        (*gel)->defined = -1; // -1 = zacala definice funkce
      }
      /* TS e*/
      (*nodeAST)->token1 = (*tkn); /// AST id
      (*tkn) = GetNextToken();
      if( (*tkn)->type != TK_BRACKET_ROUND_LEFT ) { /// (
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 ); 
      if( !Syntaxx_ListParam(tkn, &((*nodeAST)->node1), gel, isDeclareNow, checkParamNow ) ) {
        return 0;
      }
      if( (*tkn)->type != TK_BRACKET_ROUND_RIGHT ) { /// )
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_AS ) { /// as
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_DataType(tkn, &((*nodeAST)->node2), gel ) ) {
        return 0;
      }
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      return 1;
      break;
    default:
      fprintf(stderr, "FunctionHead\n");
      return 0;
      break;
  }
  
}
int Syntaxx_ListParam(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, int isDeclareNow, int checkParamNow) {
  int indexOfParam = 0; /* TS */
  (*nodeAST) = InitASTNode(AST_ListParam); // AST
  switch( (*tkn)->type ) {
    case TK_ID:
      return Syntaxx_Param(tkn, &((*nodeAST)->node1), gel, isDeclareNow, checkParamNow, indexOfParam ) && Syntaxx_NextParam(tkn, &((*nodeAST)->node2), gel, isDeclareNow, checkParamNow, indexOfParam+1 );
      break;
    case TK_BRACKET_ROUND_RIGHT:
      return 1;
      break;
    default:
      fprintf(stderr, "ListParam\n");
      return 0;
      break;
  }
  
}
int Syntaxx_Param(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, int isDeclareNow, int checkParamNow, int indexOfParam) {
  symtable_elem_t *lel = NULL; /* TS */
  TListData data; /* TS */
  (*nodeAST) = InitASTNode(AST_Param); // AST
  switch( (*tkn)->type ) {
    case TK_ID: /// id
      /* TS s*/
      lel = SymtableFind( GlobalSymtable,  (*tkn)->string ); 
      if( lel != NULL ) { // Pokud je id nazev existujici funkce tak je to chyba
        // ERR_SEM
        PrintLineErr( (*tkn) );
        fprintf(stderr, "%s neni nazev existujici funkce.\n", (*tkn)->string );
        CallError(ERR_SEM);
        return 0;
      }
      if( !isDeclareNow ) {// definice - zkontroluje existenci promnene a prida promenou do lok. tabulky
        lel = SymtableFind( (*gel)->local_symtable,  (*tkn)->string ); 
        if( lel == NULL ) { // Pokud promena neexistuje tak ji vytvor
          lel = AddElemGlobal( (*gel)->local_symtable,  (*tkn)->string );
          lel->elemType = SYM_TYPE_PARAM;
        }
        else {
          // ERR_SEM
          PrintLineErr( (*tkn) );
          fprintf(stderr, "Dublicitni nazev parametru.\n");
          CallError(ERR_SEM);
          return 0;
        }
      }
      else {
        
      }
      /* TS e*/
      (*nodeAST)->token1 = (*tkn); /// AST id
      (*tkn) = GetNextToken();
      if( (*tkn)->type != TK_AS ) { /// as
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );

      if( !isDeclareNow ) { // je definice - takze mame prvek lokalni tabulky
        if( !Syntaxx_DataType(tkn, &((*nodeAST)->node1), &lel ) ) {
          return 0;
        }
        if( !checkParamNow ) { // List parametru se teprve plni
          data.value = lel->dataType; /* TS */
          ListPushBack( (*gel)->listParam, data ); /* TS */
        }
        else { // List parametru se kontroluje
          if( !ListGet( (*gel)->listParam, indexOfParam, &data ) ) { 
            // List parametru je kradsi nez prave nacitany seznam parametru
            // ERR_SEM
            PrintLineErr( (*tkn) );
            fprintf(stderr, "List parametru v declaraci a definici nejsou stejne dlouhe.\n");
            CallError(ERR_SEM);
            return 0;
          }
          if( (st_datatype_t)data.value != lel->dataType ) { // datove typy parametru nejsou shodne
            // ERR_SEM
            PrintLineErr( (*tkn) );
            fprintf(stderr, "Datove typy v declaraci a v definici nejsou stejne.\n");
            CallError(ERR_SEM);
            return 0;
          }
        }
        (*nodeAST)->token2 = TokenInit();
        (*nodeAST)->token2->type = SymDataTypeToTokenType(lel->dataType);
      }
      else { // je deklarace - takze musime nějak ziskat datovy typ :D
        // pouzijeme jako prostrednika gel, funkce dostava totiz navratovy typ az nakonec, takze v dobe parametru je mozne jej vyuzit jako docasnou promnenou. (gel->dataType)
        if( !Syntaxx_DataType(tkn, &((*nodeAST)->node1), gel ) ) {
          return 0;
        }
        if( !checkParamNow ) { // List parametru se teprve plni
          data.value = (*gel)->dataType; /* TS */
          ListPushBack( (*gel)->listParam, data ); /* TS */
        }
        else { // ListParametru se kontroluje
          if( !ListGet( (*gel)->listParam, indexOfParam, &data ) ) { 
            // List parametru je kradsi nez prave nacitany seznam parametru
            // ERR_SEM
            PrintLineErr( (*tkn) );
            fprintf(stderr, "List parametru v declaraci a definici nejsou stejne dlouhe.\n");
            CallError(ERR_SEM);
            return 0;
          }
          if( (st_datatype_t)data.value != (*gel)->dataType ) { // datove typy parametru nejsou shodne
            // ERR_SEM
            PrintLineErr( (*tkn) );
            fprintf(stderr, "Datove typy v declaraci a definici nejsou shodne.\n");
            CallError(ERR_SEM);
            return 0;
          }
        }
        (*gel)->dataType = SYM_DATATYPE_VOID; // radsi premazeme, at nenastane nekde pozdeji chyba
      }

      return 1;
      break;
    default:
      fprintf(stderr, "Param\n");
      return 0;
      break;
  }
  
}
int Syntaxx_NextParam(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, int isDeclareNow, int checkParamNow, int indexOfParam) {
  (*nodeAST) = InitASTNode(AST_NextParam); // AST
  switch( (*tkn)->type ) {
    case TK_COMMA: /// ,
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_Param(tkn, &((*nodeAST)->node1), gel, isDeclareNow, checkParamNow, indexOfParam ) && Syntaxx_NextParam(tkn, &((*nodeAST)->node2), gel, isDeclareNow, checkParamNow, indexOfParam+1 );
      break;
    case TK_BRACKET_ROUND_RIGHT:
      return 1;
      break;
    default:
      fprintf(stderr, "NextParam\n");
      return 0;
      break;
  }
  
}
int Syntaxx_DataType(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **elem) {
  (*nodeAST) = InitASTNode(AST_DataType); // AST
  switch( (*tkn)->type ) {
    case TK_INTEGER: /// integer
    case TK_DOUBLE: /// double
    case TK_STRING: /// string
      if( (*elem)->dataType != SYM_DATATYPE_VOID ) {
        if( (*elem)->dataType != TokenTypeToSymDataType( (*tkn)->type ) ) {
          // ERR_SEM
          PrintLineErr( (*tkn) );
          fprintf(stderr, "Datovy typ funkce v deklaraci a definice se neschoduje.\n");
          CallError(ERR_SEM);
          return 0;
        }
      }
      (*elem)->dataType = TokenTypeToSymDataType( (*tkn)->type ); /* TS */
      (*nodeAST)->token1 = (*tkn); /// AST id
      (*tkn) = GetNextToken();
      //(*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
      break;
    default:
      fprintf(stderr, "DataType\n");
      return 0;
      break;
  }
  
}
int Syntaxx_FunctionEnd(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, int isDeclareNow) {
  (*nodeAST) = InitASTNode(AST_FunctionEnd); // AST
  switch( (*tkn)->type ) {
    case TK_END: /// end
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_FUNCTION ) { /// function
        fprintf(stderr, "end function\n");
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      if( !isDeclareNow ) {
        (*gel)->defined = 1; // Skoncila definice fukce
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      return 1;
      break;
    default:
      fprintf(stderr, "FunctionEnd\n");
      return 0;
      break;
  }
  
}
int Syntaxx_ScopeDef(TToken **tkn, TATSNode **nodeAST) {
  symtable_elem_t *gel = NULL; /* TS */
  (*nodeAST) = InitASTNode(AST_ScopeDef); // AST
  switch( (*tkn)->type ) {
    case TK_SCOPE:
      return Syntaxx_ScopeHead(tkn, &((*nodeAST)->node1), &gel ) && Syntaxx_FunctionBody(tkn, &((*nodeAST)->node2), &gel ) && Syntaxx_ScopeEnd(tkn, &((*nodeAST)->node3) );
      break;
    default:
      fprintf(stderr, "ScopeDef\n");
      return 0;
      break;
  }
  
}
int Syntaxx_ScopeHead(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel) {
  (*nodeAST) = InitASTNode(AST_ScopeHead); // AST
  switch( (*tkn)->type ) {
    case TK_SCOPE: /// scope
      /* TS s*/
      (*gel) = SymtableFind(GlobalSymtable,  (*tkn)->string ); // jen scope muze se jmenovat scope
      if( (*gel) == NULL ) { // Pokud funkce neexistuje tak ji vytvor
        (*gel) = AddElemGlobal(GlobalSymtable,  (*tkn)->string );
        (*gel)->elemType = SYM_TYPE_FUNCTION;
        (*gel)->dataType = SYM_DATATYPE_VOID;
        (*gel)->listParam = ListInit();
      }
      else {
        // ERR_SYN
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Opakovana definice scope.\n");
        return 0;
      }
      /* TS e*/
      (*nodeAST)->token1 = (*tkn); /// AST scope
      (*tkn) = GetNextToken();
      //(*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      return 1;
      break;
    default:
      fprintf(stderr, "ScopeHead\n");
      return 0;
      break;
  }
  
}
int Syntaxx_ScopeEnd(TToken **tkn, TATSNode **nodeAST) {
  (*nodeAST) = InitASTNode(AST_ScopeEnd); // AST
  switch( (*tkn)->type ) {
    case TK_END: /// end
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_SCOPE ) { /// scope
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_ScopeAfter(tkn, &((*nodeAST)->node1) );
      break;
    default:
      fprintf(stderr, "ScopeEnd\n");
      return 0;
      break;
  }
  
}
int Syntaxx_ScopeAfter(TToken **tkn, TATSNode **nodeAST) {
  (*nodeAST) = InitASTNode(AST_ScopeAfter); // AST
  switch( (*tkn)->type ) {
    case TK_EOL: /// eol
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      return Syntaxx_ScopeAfter(tkn, &((*nodeAST)->node1) );
      break;
    case TK_EOF: /// eof
      return 1;
      break;
    default:
      fprintf(stderr, "ScopeAfter\n");
      return 0;
      break;
  }
  
}
int Syntaxx_FunctionBody(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel) {
  (*nodeAST) = InitASTNode(AST_FunctionBody); // AST
  switch( (*tkn)->type ) {
    case TK_ID:
    case TK_END:
    case TK_DIM:
    case TK_DO:
    case TK_IF:
    case TK_INPUT:
    case TK_PRINT:
    case TK_RETURN:
      return Syntaxx_ListVarDef(tkn, &((*nodeAST)->node1), gel ) && Syntaxx_ListCommand(tkn, &((*nodeAST)->node2), gel );
      break;
    default:
      fprintf(stderr, "FunctionBody\n");
      return 0;
      break;
  }
  
}
int Syntaxx_ListVarDef(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel) {
  symtable_elem_t *lel = NULL; /* TS */
  (*nodeAST) = InitASTNode(AST_ListVarDef); // AST
  switch( (*tkn)->type ) {
    case TK_DIM: /// dim
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_ID ) { /// id
        return 0;
      }
      /* TS s*/
      lel = SymtableFind( GlobalSymtable,  (*tkn)->string ); 
      if( lel != NULL ) { // Nazev promene je stejny jako jmeno existujici funkce
        // ERR_SEM
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Nazev promene '%s' je stejny jako jmeno existujici funkce.\n", (*tkn)->string );
        CallError(ERR_SEM);
        return 0;
      }
      lel = SymtableFind( (*gel)->local_symtable,  (*tkn)->string ); 
      if( lel == NULL ) { // Pokud promena neexistuje tak ji vytvor
        lel = AddElemGlobal( (*gel)->local_symtable,  (*tkn)->string );
        lel->elemType = SYM_TYPE_VAR;
      }
      else {
        // ERR_SEM
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Dublicitni nazev promene '%s'.\n", (*tkn)->string );
        CallError(ERR_SEM);
        return 0;
      }
      /* TS e*/
      (*nodeAST)->token1 = (*tkn); /// AST id
      (*tkn) = GetNextToken();
      //(*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_AS ) { /// as
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_DataType(tkn, &((*nodeAST)->node1), &lel ) ) { 
        return 0;
      }
      if( !Syntaxx_VarDefAssigment(tkn, &((*nodeAST)->node2), gel, SymDataTypeToTokenType( lel->dataType ) ) ) {
        return 0;
      }
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      return Syntaxx_ListVarDef(tkn, &((*nodeAST)->node3), gel ); 
      break;
    case TK_ID:
    case TK_END:
    case TK_DO:
    case TK_IF:
    case TK_INPUT:
    case TK_PRINT:
    case TK_RETURN:
      return 1;
      break;
    default:
      fprintf(stderr, "ListVarDef\n");
      return 0;
      break;
  }
  
}

int Syntaxx_VarDefAssigment(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, TTokenType tokenType) {
  (*nodeAST) = InitASTNode(AST_VarDefAssigment); // AST
  (*nodeAST)->token1 = TokenInit();
  (*nodeAST)->token1->type = tokenType;
  switch( (*tkn)->type ) {
    case TK_EQUAL: /// =
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_Expression(tkn, nodeAST, gel) ) { // TK_EXPRESSION
        return 0;
      }
      // Kontrola zda je mozne priradit vysledek vyrazu do promnene.
      if( ( (*nodeAST)->token1->type == TK_NUM_INTEGER ) && ( (*nodeAST)->token2->type == TK_NUM_DOUBLE ) ) {
      }
      else if( ( (*nodeAST)->token1->type == TK_NUM_DOUBLE ) && ( (*nodeAST)->token2->type == TK_NUM_INTEGER ) ) {
      }
      else if( (*nodeAST)->token1->type != (*nodeAST)->token2->type ) {
        // ERR_COMP
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Promnena je jineho datoveho typu nez vysledny vyraz.\n");
        CallError(ERR_COMP);
        return 0;
      }
      return 1;
      break;
    case TK_EOL:
      return 1;
      break;
    default:
      fprintf(stderr, "VarDefAssigment\n");
      return 0;
      break;
  }
}

int Syntaxx_ListCommand(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel) {
  (*nodeAST) = InitASTNode(AST_ListCommand); // AST
  switch( (*tkn)->type ) {
    case TK_ID:
    case TK_DO:
    case TK_IF:
    case TK_INPUT:
    case TK_PRINT:
    case TK_RETURN:
      if( !Syntaxx_Command(tkn, &((*nodeAST)->node1), gel ) ) {
        return 0;
      }
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      return Syntaxx_ListCommand(tkn, &((*nodeAST)->node2), gel );
      break;
    case TK_END:
    case TK_LOOP:
    case TK_ELSE:
      return 1;
      break;
    default:
      fprintf(stderr, "ListCommand\n");
      return 0;
      break;
  }
  
}
int Syntaxx_Command(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel) {
  symtable_elem_t *lel = NULL; /* TS */
  (*nodeAST) = InitASTNode(AST_Command); // AST
  switch( (*tkn)->type ) {
    case TK_ID: /// id
      /* TS s*/
      lel = SymtableFind( (*gel)->local_symtable,  (*tkn)->string ); 
      if( lel == NULL ) { // Nejze priradit do promene ktera neexistuje
        // ERR_SEM
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Promena '%s' neexistuje.\n", (*tkn)->string );
        CallError(ERR_SEM);
        return 0;
      }
      (*nodeAST)->token2 = TokenInit();
      (*nodeAST)->token2->type = SymDataTypeToTokenType(lel->dataType);
      /* TS e*/
      (*nodeAST)->token1 = (*tkn); /// AST id
      (*tkn) = GetNextToken();
      //(*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_EQUAL ) { /// =
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_Assignment(tkn, &((*nodeAST)->node1), gel ) ) {
        return 0;
      }
      // PrintToken((*nodeAST)->token2); // DEBUG
      // PrintToken(((*nodeAST)->node1)->token2); // DEBUG
      // Kontrola zda je mozne priradit vypocitanou/vracenou hodnotu do promene.
      if( ( (*nodeAST)->token2->type == TK_NUM_INTEGER) && ( ((*nodeAST)->node1)->token2->type == TK_NUM_DOUBLE ) ) {
        // integer - double
      }
      else if( ( (*nodeAST)->token2->type == TK_NUM_DOUBLE) && ( ((*nodeAST)->node1)->token2->type == TK_NUM_INTEGER ) ) {
        // double - integer
      }
      else if( (*nodeAST)->token2->type != ((*nodeAST)->node1)->token2->type ) {
        // ERR_COMP
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Promnena je jineho datoveho typu nez vysledny vyraz.\n");
        CallError(ERR_COMP);
        return 0;
      }
      return 1;
      break;
    case TK_DO: /// do
      (*nodeAST)->token1 = (*tkn); /// AST do
      (*tkn) = GetNextToken();
      //(*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_WHILE ) { /// while
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_Condition(tkn, &((*nodeAST)->node1), gel ) ) {
        return 0;
      }
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      if( !Syntaxx_ListCommand(tkn, &((*nodeAST)->node2), gel ) ) {
        return 0;
      }
      if( (*tkn)->type != TK_LOOP ) { /// loop
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
      break;
    case TK_IF: /// if
      (*nodeAST)->token1 = (*tkn); /// AST if
      (*tkn) = GetNextToken();
      //(*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_Condition(tkn, &((*nodeAST)->node1), gel ) ) { 
        return 0;
      }
      if( (*tkn)->type != TK_THEN ) { /// then
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      if( !Syntaxx_ListCommand(tkn, &((*nodeAST)->node2), gel ) ) {
        return 0;
      }
      if( (*tkn)->type != TK_ELSE ) { /// else
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_EOL ) { /// eol
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),0 );
      if( !Syntaxx_ListCommand(tkn, &((*nodeAST)->node3), gel ) ) {
        return 0;
      }
      if( (*tkn)->type != TK_END ) { /// end
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_IF ) { /// if
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
      break;
    case TK_INPUT: /// input
      (*nodeAST)->token1 = (*tkn); /// AST input
      (*tkn) = GetNextToken();
      //(*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_ID ) { /// id
        return 0;
      }
      /* TS s*/
      lel = SymtableFind( (*gel)->local_symtable,  (*tkn)->string ); 
      if( lel == NULL ) { // Nelze nacitat do promene ktera neexistuje
        // ERR_SEM
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Prmonena '%s' neexistuje.\n", (*tkn)->string );
        CallError(ERR_SEM);
        return 0;
      }
      /* TS e*/
      (*nodeAST)->token2 = (*tkn); /// AST input id
      (*nodeAST)->token3 = TokenInit();
      (*nodeAST)->token3->type = SymDataTypeToTokenType( lel->dataType ); 
      (*tkn) = GetNextToken();

      return 1;
      break;
    case TK_PRINT: /// print
      (*nodeAST)->token1 = (*tkn); /// AST print
      (*tkn) = GetNextToken();

      return Syntaxx_ListExpression(tkn, &((*nodeAST)->node1), gel );
      break;
    case TK_RETURN: /// return
      (*nodeAST)->token1 = (*tkn); /// AST return
      (*tkn) = GetNextToken();

      if( !Syntaxx_Expression(tkn, nodeAST, gel) ) { 
        return 0;
      }
      (*nodeAST)->token3 = TokenInit();
      (*nodeAST)->token3->type = SymDataTypeToTokenType( (*gel)->dataType );

      // Kontrola zda je mozne priradit vypocitanou/vracenou hodnotu do promene.
      if( (*nodeAST)->token2->type == TK_NUM_INTEGER && SymDataTypeToTokenType( (*gel)->dataType ) == TK_NUM_DOUBLE ) {
        // integer double
      }
      else if( (*nodeAST)->token2->type == TK_NUM_DOUBLE && SymDataTypeToTokenType( (*gel)->dataType ) == TK_NUM_INTEGER ) {
        // double integer
      }
      else if( (*nodeAST)->token2->type != SymDataTypeToTokenType( (*gel)->dataType ) ) {
        // ERR_COMP
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Datovy typ vraceneho vysledneho vyrazu je jiny nez datovy typ funkce.\n");
        CallError(ERR_COMP);
        return 0;
      }
      return 1;
      break;
    default:
      fprintf(stderr, "Command\n");
      return 0;
      break;
  }
  return 0;
}
int Syntaxx_ListExpression(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel) {
  (*nodeAST) = InitASTNode(AST_ListExpression); // AST
  // Expression
  if( (*tkn)->type == TK_ID || (*tkn)->type == TK_NUM_INTEGER || (*tkn)->type == TK_NUM_DOUBLE || (*tkn)->type == TK_NUM_STRING || (*tkn)->type == TK_BRACKET_ROUND_LEFT ) {

    if( Syntaxx_Expression(tkn, nodeAST, gel) ) {
      if( (*tkn)->type != TK_SEMICOLON ) {
        fprintf(stderr, "Chyby strednik za vyrazem print.\n");
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return Syntaxx_ListExpression(tkn, &((*nodeAST)->node1), gel ); // AST - Mozna zmnena podle toho jak upravim Expression
    }
  }
  switch( (*tkn)->type ) {
    case TK_EOL: 
      return 1;
      break;
    default:
      fprintf(stderr, "ListExpression\n");
      return 0;
      break;
  }
  
}
int Syntaxx_Condition(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel) {
  (*nodeAST) = InitASTNode(AST_Condition); // AST
  (*nodeAST)->token1 = TokenInit();
  (*nodeAST)->token1->type = TK_TRUE; 
  // Expression
  if( (*tkn)->type == TK_ID || (*tkn)->type == TK_NUM_INTEGER || (*tkn)->type == TK_NUM_DOUBLE || (*tkn)->type == TK_NUM_STRING || (*tkn)->type == TK_BRACKET_ROUND_LEFT ) {
    if( !Syntaxx_Expression(tkn, nodeAST, gel) ) {
      return 0;
    } 
    // Kontrola zda vysledek vyrazu je pravdivostni hodnota - bool
    if( (*nodeAST)->token1->type != (*nodeAST)->token2->type ) {
      // ERR_COMP
      PrintLineErr( (*tkn) );
      fprintf(stderr, "Vysledek vyrazu v podmince musi byt true nebo false.\n");
      CallError(ERR_COMP);
      return 0;
    }
    return 1;
  }
  fprintf(stderr, "Condition\n");
  return 0;
  
}
int Syntaxx_Assignment(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel) {
  symtable_elem_t *lel = NULL; /* TS */
  (*nodeAST) = InitASTNode(AST_Assignment); // AST
  // Expression
  if( (*tkn)->type == TK_ID || (*tkn)->type == TK_NUM_INTEGER || (*tkn)->type == TK_NUM_DOUBLE || (*tkn)->type == TK_NUM_STRING || (*tkn)->type == TK_BRACKET_ROUND_LEFT ) {
    if( Syntaxx_Expression(tkn, nodeAST, gel) ) {
      return 1;
    } // Pokud se nepovede tak nedelej nic
  }
  switch( (*tkn)->type ) {
    case TK_LENGTH: /// length
    case TK_SUBSTR: /// substr
    case TK_ASC: /// asc
    case TK_CHR: /// chr
    case TK_ID: /// id
      /* TS s*/
      lel = SymtableFind( GlobalSymtable,  (*tkn)->string ); 
      if( lel == NULL ) { // ID musi byt funkce. Muzes volat jen funkce ktere existuji.
        // ERR_SEM
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Funkce '%s' neexistuje.\n", (*tkn)->string );
        CallError(ERR_SEM);
        return 0;
      }
      else if( lel->defined == -1 && lel->declared == 0 ) {
        // ERR_SEM
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Ve funkci '%s' nemuze byt volana funkce '%s' pokud ji nepredchazi declarace.\n", (*tkn)->string, (*tkn)->string );
        CallError(ERR_SEM);
        return 0;
      }
      fprintf(stderr, "X>> %d\n",lel->defined );
      (*nodeAST)->token2 = TokenInit();
      (*nodeAST)->token2->type = SymDataTypeToTokenType(lel->dataType);
      /* TS e*/
      (*nodeAST)->token1 = (*tkn); /// AST id | lenght | substr | asc | chr
      (*tkn) = GetNextToken();
      //(*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( (*tkn)->type != TK_BRACKET_ROUND_LEFT ) { /// (
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      if( !Syntaxx_ListInParam(tkn, &((*nodeAST)->node1), gel, &lel ) ) {
        return 0;
      }
      if( (*tkn)->type != TK_BRACKET_ROUND_RIGHT ) { /// )
        return 0;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );
      return 1;
      break;
    default:
      fprintf(stderr, "Assignment\n");
      return 0;
      break;
  }
  return 0;
}
int Syntaxx_ListInParam(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, symtable_elem_t **fel) {
  (*nodeAST) = InitASTNode(AST_ListInParam); // AST
  switch( (*tkn)->type ) {
    case TK_ID:
    case TK_NUM_INTEGER:
    case TK_NUM_DOUBLE:
    case TK_NUM_STRING:
      return Syntaxx_InParam(tkn, &((*nodeAST)->node1), gel, fel, 0 ) && Syntaxx_NextInParam(tkn, &((*nodeAST)->node2), gel, fel, 1 );
      break;
    case TK_BRACKET_ROUND_RIGHT:

      if( (*fel)->listParam->count > 0 ) {
        // ERR_SYN
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Bylo zadano mene parametru nez kolik funkce prijima.\n");
        return 0;
      }

      return 1;
      break;
    default:
      fprintf(stderr, "ListInParam\n");
      return 0;
      break;
  }
  
}
int Syntaxx_InParam(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, symtable_elem_t **fel, int indexOfParam) {
  TListData data; /* TS */
  (*nodeAST) = InitASTNode(AST_InParam); // AST
  switch( (*tkn)->type ) {
    case TK_ID:
    case TK_NUM_INTEGER:
    case TK_NUM_DOUBLE:
    case TK_NUM_STRING:
      if( !Syntaxx_Term(tkn, &((*nodeAST)->node1), gel ) ) {
        return 0;
      }
      (*nodeAST)->token2 = TokenInit();
      (*nodeAST)->token2->type = ((*nodeAST)->node1)->token2->type;
      if( !ListGet( (*fel)->listParam , indexOfParam, &data ) ) {
        // ERR_SYN
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Bylo zadano vice parametru nez kolik funkce prijima.\n");
        return 0;
      }
      (*nodeAST)->token1 = TokenInit();
      (*nodeAST)->token1->type = SymDataTypeToTokenType(data.value);
      // Kontrola zda mohou byt vkladane hodnoty na pozici parametru - kontrola typu promene a vkladane hodnoty
      if( ( (*nodeAST)->token1->type == TK_NUM_INTEGER ) && ( (*nodeAST)->token2->type == TK_NUM_DOUBLE ) ) {
        // integer - double
      }
      else if( ( (*nodeAST)->token1->type == TK_NUM_DOUBLE ) && ( (*nodeAST)->token2->type == TK_NUM_INTEGER ) ) {
        // double - integer
      }
      else if( (*nodeAST)->token1->type != (*nodeAST)->token2->type ) {
        // ERR_COMP
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Parametr je jineho datoveho typu, nez ktery funkce ocekava.\n");
        CallError(ERR_COMP);
        return 0;
      }
      return 1;
      break;
    default:
      fprintf(stderr, "InParam\n");
      return 0;
      break;
  }
  
}
int Syntaxx_NextInParam(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, symtable_elem_t **fel, int indexOfParam) {
  (*nodeAST) = InitASTNode(AST_NextInParam); // AST
  switch( (*tkn)->type ) {
    case TK_COMMA: /// ,
      (*tkn) = GetNextDestroyOldToken( (*tkn),1 );

      return Syntaxx_InParam(tkn, &((*nodeAST)->node1), gel, fel, indexOfParam ) && Syntaxx_NextInParam(tkn, &((*nodeAST)->node2), gel, fel, indexOfParam+1 );
      break;
    case TK_BRACKET_ROUND_RIGHT:

      if( (*fel)->listParam->count > indexOfParam ) {
        // ERR_SYN
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Bylo zadano mene parametru nez kolik funkce prijima.\n");
        return 0;
      }

      return 1;
      break;
    default:
      fprintf(stderr, "NextInParam\n");
      return 0;
      break;
  }
  
}
int Syntaxx_Term(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel) {
  symtable_elem_t *lel = NULL; /* TS */
  (*nodeAST) = InitASTNode(AST_Term); // AST
  switch( (*tkn)->type ) {
    case TK_ID: /// id
      /* TS s*/
      lel = SymtableFind( (*gel)->local_symtable,  (*tkn)->string ); 
      if( lel == NULL ) { // promene neexistuje
        // ERR_SEM
        PrintLineErr( (*tkn) );
        fprintf(stderr, "Promnena '%s' neexistuje.\n", (*tkn)->string );
        CallError(ERR_SEM);
        return 0;
      }
      (*nodeAST)->token2 = TokenInit();
      (*nodeAST)->token2->type = SymDataTypeToTokenType(lel->dataType);
      /* TS e*/
    case TK_NUM_INTEGER: /// int 5
    case TK_NUM_DOUBLE: /// float 2.3
    case TK_NUM_STRING: /// str !"string"
      (*nodeAST)->token1 = (*tkn); /// AST id | int | float | str
      if( (*tkn)->type != TK_ID ) {
        (*nodeAST)->token2 = TokenInit();
        (*nodeAST)->token2->type = (*tkn)->type;
      }
      (*tkn) = GetNextToken();
      return 1;
      break;
    default:
      fprintf(stderr, "Term\n");
      return 0;
      break;
  }
  
}




//----------------------------------------
// Syntaxx_Expression
// ---------------------------------------

/*_____________
  |       |   |
  |   * / | 1 |
  | \ mod | 2 |
  |   + - | 3 |
  |     ( | 4 |
  |_______|___|
 */
int PriorityOperator(TTokenType type) {
  switch(type) {
    case TK_BRACKET_ROUND_LEFT:
      return 5;
    case TK_PLUS:
    case TK_MINUS:
      return 3;
    case TK_MUL:
    case TK_DIV:
      return 1;
    //case TK_MOD:
    case TK_DIV_INT:
      return 2;
    case TK_LESS: 
    case TK_LESS_EQUAL: 
    case TK_GREATER: 
    case TK_GREATER_EQUAL:
    case TK_EQUAL: 
    case TK_NOT_EQUAL: 
      return 4; 
    default:
      return -1;
  }
}

// Prevod vyrazu z infix na posfix - Muze byt operator vlozen na zasobnik operatoru?
int CanBeOperatorPush(TTokenType operatorNow, TTokenType operatorOnStack ) {
  if( operatorNow == TK_BRACKET_ROUND_LEFT ) 
    return 1;
  if( PriorityOperator(operatorNow) < PriorityOperator(operatorOnStack) ) 
    return 1;
  return 0;
}

int IsOperator(TTokenType type) {
  switch(type) {
    case TK_PLUS:
    case TK_MINUS:
    case TK_MUL:
    case TK_DIV:
    //case TK_MOD:
    case TK_DIV_INT:
    case TK_LESS: 
    case TK_LESS_EQUAL: 
    case TK_GREATER: 
    case TK_GREATER_EQUAL:
    case TK_EQUAL: 
    case TK_NOT_EQUAL:  
      return 1;
      break;
    default:
      return 0;
      break;
  }
}

int IsOperatorCompare(TTokenType type) {
  switch(type) {
    case TK_LESS: 
    case TK_LESS_EQUAL: 
    case TK_GREATER: 
    case TK_GREATER_EQUAL:
    case TK_EQUAL: 
    case TK_NOT_EQUAL:  
      return 1;
      break;
    default:
      return 0;
      break;
  }
}

// Prevod vyrazu z infix na posfix - Muze nasledovat token ... za tokenem ...?
int CanBeTokenAfterToken(TTokenType now, TTokenType last) {
  switch(last) {
    case TK_ID:
    case TK_NUM_INTEGER:
    case TK_NUM_DOUBLE:
    case TK_NUM_STRING:
    case TK_BRACKET_ROUND_RIGHT:
      switch(now) {
        case TK_PLUS:
        case TK_MINUS:
        case TK_MUL:
        case TK_DIV:
        //case TK_MOD:
        case TK_DIV_INT:
        case TK_BRACKET_ROUND_RIGHT:
        case TK_LESS: 
        case TK_LESS_EQUAL: 
        case TK_GREATER: 
        case TK_GREATER_EQUAL:
        case TK_EQUAL: 
        case TK_NOT_EQUAL: 
          return 1;
        default:
          return 0;
      }
    case TK_BRACKET_ROUND_LEFT:
    case TK_PLUS:
    case TK_MINUS:
    case TK_MUL:
    case TK_DIV:
    //case TK_MOD:
    case TK_DIV_INT:
    case TK_LESS: 
    case TK_LESS_EQUAL: 
    case TK_GREATER: 
    case TK_GREATER_EQUAL:
    case TK_EQUAL: 
    case TK_NOT_EQUAL:  
      switch(now) {
        case TK_ID:
        case TK_NUM_INTEGER:
        case TK_NUM_DOUBLE:
        case TK_NUM_STRING:
        case TK_BRACKET_ROUND_LEFT:
          return 1;
        default:
          return 0;
      }
    default:
      break;
  }
  return 0;
}

int CanBeTokenInExpression(TTokenType type) {
  switch(type) {
    case TK_ID:
    case TK_NUM_INTEGER:
    case TK_NUM_DOUBLE:
    case TK_NUM_STRING:
    case TK_BRACKET_ROUND_RIGHT:
    case TK_BRACKET_ROUND_LEFT:
    case TK_PLUS:
    case TK_MINUS:
    case TK_MUL:
    case TK_DIV:
    //case TK_MOD:
    case TK_DIV_INT:
      return 1;
      break;
    default:
      return 0;
      break;
  }

}

int Syntaxx_Expression( TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel)  {
  (void)gel; /* TS TODO */
  int returnValue = 1; // Navratova hodnota
  TList *StackOperator; // Stack Tokenu
  TList *ListPostFix; // List Tokenu
  StackOperator = ListInit();
  ListPostFix = ListInit();

  if(StackOperator == NULL || ListPostFix == NULL) {
    // ERR_INTERNAL
    CallError(ERR_INTERNAL);   
    return 0;
  }

  TTokenType lastToken = TK_BRACKET_ROUND_LEFT;
  int NumberOfOperand = 0; // Pocet Operandu tj TK_ID,TK_NUM_*
  int NumberOfOperator = 0; // Pocet Operatoru tj TK_PLUS, ...
  TTokenType lastOperatorTokenType = TK_PLUS; // Slouzi jen k detekci deleni nulou

  do {

    if( CanBeTokenInExpression( (*tkn)->type ) ) {
      if( !CanBeTokenAfterToken( (*tkn)->type, lastToken ) ) {
        // neocekavany token vyrazu
        // fprintf(stderr, ">Expression: Neocekavany token, ale nejedna se o chybu.\n");// DEBUG
        returnValue = 0;
        RepeatLastToken();
        TListData data;
        if( ListPopBack(ListPostFix, &data) ) {
          TokenDestroy( (*tkn) );
          (*tkn) = (data.pointer);
        }
        break; 
      }
    }
    lastToken =  (*tkn)->type;

    if(  (*tkn)->type == TK_ID ) {
      TListData data;
      data.pointer = (*tkn);
      ListPushBack(ListPostFix, data);
      NumberOfOperand++;
    }
    else if( (*tkn)->type == TK_NUM_INTEGER || (*tkn)->type == TK_NUM_DOUBLE || (*tkn)->type == TK_NUM_STRING ) {

      /// DIV ZERO - start
      if( lastOperatorTokenType == TK_DIV || lastOperatorTokenType == TK_DIV_INT ) {
        if( (*tkn)->type == TK_NUM_INTEGER ) {
          if( (*tkn)->data.integerValue == 0 ) {
            /// ERR_SEM_OTHER
            PrintLineErr( (*tkn) );
            fprintf(stderr, "Deleni nulou.\n" );
            CallError(ERR_SEM_OTHER);
            break;
          }
        }
        else if( (*tkn)->type == TK_NUM_DOUBLE ) {
          if( (*tkn)->data.doubleValue == 0.0 ) {
            /// ERR_SEM_OTHER
            PrintLineErr( (*tkn) );
            fprintf(stderr, "Deleni nulou\n" );
            CallError(ERR_SEM_OTHER);
            break;
          }
        }
      }
      /// DIV ZERO - end
      
      TListData data;
      data.pointer = (*tkn);
      ListPushBack(ListPostFix, data);
      NumberOfOperand++;
    }
    else if(  (*tkn)->type == TK_BRACKET_ROUND_LEFT ) {
      TListData data;
      data.pointer = (*tkn);
      ListPush(StackOperator, data);
    }
    else if( IsOperator( (*tkn)->type ) ) {
      // Podle priority operatoru zpracuj operatory
      lastOperatorTokenType = (*tkn)->type;
      TListData data;
      while( ListFront(StackOperator, &data) && !CanBeOperatorPush( (*tkn)->type, ( (TToken*)data.pointer)->type ) ) {
        ListPushBack(ListPostFix, data);
        ListPop(StackOperator, &data);
      }//- while
      data.pointer = (*tkn);
      ListPush(StackOperator, data);
      NumberOfOperator++;
    }
    else if(  (*tkn)->type == TK_BRACKET_ROUND_RIGHT ) {
      TListData data;
      while(1) {
        if( !ListPop(StackOperator, &data) ) {
          // Pokud zasobnik op Neobsahuje '(' 
          // ERR_SYN
          PrintLineErr( (*tkn) );
          fprintf(stderr, ">Expression: Vyraz obsahuje prebytecnou zavorku ')'\n");
          CallError(ERR_SYN);
          break;
        }
        else if( ((TToken*)data.pointer)->type == TK_BRACKET_ROUND_LEFT ) {
          TokenDestroy( ((TToken*)data.pointer) ); // Uvolnim token zavorky ( protoze ho uz nepotrebuji
          break;
        }
        else {
          ListPushBack(ListPostFix, data); // dokud nenajde '(' tak operace sklada na vystupni retezec
        }

      }//-while
      TokenDestroy( (*tkn) ); // Uvolnim token zavorky ) protoze ho uz nepotrebuji
    }
    else {
      TListData data;
      while( ListPop(StackOperator, &data) ) {
        if( ((TToken*)data.pointer)->type == TK_BRACKET_ROUND_LEFT ) {
          // ERR_SYN
          PrintLineErr( (*tkn) );
          fprintf(stderr, ">Expression: Neocekavana zavorka '('.\n");
          CallError(ERR_SYN);
          break;
        }
        else {
          ListPushBack(ListPostFix, data);
        }
      }//-while
      break; // Uspesny konec
    }

    (*tkn) = GetNextToken(); // Jen novy token ale stary neuvolnovat nebot je v ListPostFix nebo v StackOperator
  } while(!ERR_EXIT_STATUS);

  if( returnValue && NumberOfOperand != (NumberOfOperator + 1) ) {
    PrintLineErr( (*tkn) );
    fprintf(stderr, ">Expression: Na konci vyrazu je operator.\n");
    CallError(ERR_SYN);
  }

  if( returnValue && ListEmpty(ListPostFix) ) { // Pokud je vystupni Vyraz prazdny, tak neexistoval
    PrintLineErr( (*tkn) );
    fprintf(stderr, ">Expression: Nebyl zadan vyraz.\n");
    CallError(ERR_SYN);
  }
  if( returnValue && !ListEmpty(StackOperator) ) {
    PrintLineErr( (*tkn) );
    fprintf(stderr, ">Expression: Nespravny vyraz zkontrolujte pocet operandu a operatoru.\n");
    CallError(ERR_SYN);
  }


  TListData data;

  // ListPostFix obsahuje vyraz v posix podobe. (Pro pozdejsi zpracovani kodu na stacku)
  if( !ERR_EXIT_STATUS && returnValue ) {
    // Semanticka kontrola
    Semantic_ControlExpression( &ListPostFix, nodeAST, gel );

    (*nodeAST)->listPostFix = ListPostFix; // AST - listPostFix
  }
  else { // Uvolneni pameti pokud se nejedna o vyraz nebo doslo k chybe
    //fprintf(stdout, ">>>ListPostFix\n");// DEBUG
    while( ListPop(ListPostFix, &data) ) {
      //PrintToken( ((TToken*)data.pointer) );// DEBUG
      TokenDestroy( ((TToken*)data.pointer) );
    }
    ListDestroy(ListPostFix);
  }
  // Uvolneni pameti ve stacku operatoru - pokud doslo k chybe, tak neude totiz prazdny.
  //fprintf(stdout, ">>>StackOperator\n");// DEBUG
  while( ListPop(StackOperator, &data) ) {
    //PrintToken( ((TToken*)data.pointer) );// DEBUG
    TokenDestroy( ((TToken*)data.pointer) );
  }
  //fprintf(stdout, ">>>\n");// DEBUG
  ListDestroy(StackOperator);

  return !ERR_EXIT_STATUS && returnValue;
} //- int Syntaxx_Expression

// ---------------------------------
// Semanticka kontrola
// ---------------------------------

int Semantic_ControlExpression( TList **listPostFix, TATSNode **nodeAST, symtable_elem_t **gel ) {
  TList *stack;
  stack = ListInit();

  TListData data;
  TListData tmpData;
  int i = 0;

  for(i = 0; i < (*listPostFix)->count; i++ ) {
    ListGet( (*listPostFix), i, &data );
    if( ((TToken*)data.pointer)->type == TK_ID ) {
      /* TS s*/
      symtable_elem_t *lel = NULL;
      lel = SymtableFind( (*gel)->local_symtable,  ((TToken*)data.pointer)->string ); 
      if( lel == NULL ) { // Promene neexistuje
        // ERR_SEM
        PrintLineErr( ((TToken*)data.pointer) );
        fprintf(stderr, "Promena '%s' neexistuje\n",((TToken*)data.pointer)->string );
        CallError(ERR_SEM);
        return 0;
      }
      /* TS e*/
      tmpData.value = SymDataTypeToTokenType( lel->dataType ); // Datovy typ ale ve formatu typu tokenu TK_NUM_* (pr TK_INTEGER prevede na TK_NUM_INTEGER - pro snazsi porovnani)
      tmpData.i = i; // Index kde se polozka nachazi v listu
      ListPush( stack, tmpData );
    }
    else if( ((TToken*)data.pointer)->type == TK_NUM_INTEGER || ((TToken*)data.pointer)->type == TK_NUM_DOUBLE || ((TToken*)data.pointer)->type == TK_NUM_STRING ) {
      tmpData.value = ((TToken*)data.pointer)->type ; // typ tokenu TK_NUM_* (datovy typ)
      tmpData.i = i; // Index kde se polozka nachazi v listu
      ListPush( stack, tmpData );
    }
    else if( IsOperator( ((TToken*)data.pointer)->type ) ) {
      
      TListData x1;
      TListData x2;
      ListPop( stack, &x2);
      ListPop( stack, &x1);

      /// Chyba pokud neni: string + string, stirng > string, string = strng, ...
      if( ( x1.value == TK_NUM_STRING && x2.value == TK_NUM_STRING ) && ( ((TToken*)data.pointer)->type != TK_PLUS  && !IsOperatorCompare(((TToken*)data.pointer)->type) ) ) {
        // string + string OK 
        // string compare string OK
        // jinak chyba
        // ERR_COMP
        PrintLineErr( ((TToken*)data.pointer) );
        fprintf(stderr, "Mezi datovym typem string jsou povoleny operace +, <, >, <>, =\n" );
        CallError(ERR_COMP);
        break;
      }
      /// Chyba pokud neni: int \ int = int
      else if ( (((TToken*)data.pointer)->type == TK_DIV_INT ) && ( x1.value != TK_NUM_INTEGER || x2.value != TK_NUM_INTEGER ) ) {
        // ERR_COMP
        PrintLineErr( ((TToken*)data.pointer) );
        fprintf(stderr, "Celociselne deleni je mozne provadet jen s datovym typem integer.\n");
        CallError(ERR_COMP);
        break;
      }
      /// Prevod int na double: int + double = double, int > double = bool
      else if( x1.value == TK_NUM_INTEGER && x2.value == TK_NUM_DOUBLE ) { // Na vrcholu zasobniku je double a za nim nasleduje int ( v postFixu: int,double,operator )
        tmpData.pointer = TokenInit();
        ((TToken*)tmpData.pointer)->type = TK_INT2FLOAT; 
        i++;
        ListInsert( (*listPostFix), x1.i+1 , tmpData ); // Vlozim Int2Float do listu PostFix vyrazu na pozici za prevadenou hodnotu
        tmpData.pointer = NULL;
        tmpData.value = ( IsOperatorCompare(((TToken*)data.pointer)->type) )? TK_TRUE : TK_NUM_DOUBLE;
        tmpData.i = i;
        ListPush( stack, tmpData );
      }
      /// Prevod int na double: double + int = double, double > int = bool
      else if( x1.value == TK_NUM_DOUBLE && x2.value == TK_NUM_INTEGER ) { // Na vrcholu zasobniku je int a za nim nasleduje double ( v postFixu: double,int,operator )
        tmpData.pointer = TokenInit();
        ((TToken*)tmpData.pointer)->type = TK_INT2FLOAT;
        i++;
        ListInsert( (*listPostFix), x2.i+1 , tmpData ); // Vlozim Int2Float do listu PostFix vyrazu na pozici za prevadenou hodnotu
        tmpData.pointer = NULL;
        tmpData.value = ( IsOperatorCompare(((TToken*)data.pointer)->type) )? TK_TRUE : TK_NUM_DOUBLE;
        tmpData.i = i;
        ListPush( stack, tmpData );
      }
      /// int / int = double
      else if( ( ((TToken*)data.pointer)->type == TK_DIV ) && ( x1.value == TK_NUM_INTEGER && x2.value == TK_NUM_INTEGER ) ) {
        // Prevede obe cisla na double a nasledne muze projit deleni
        tmpData.pointer = TokenInit();
        ((TToken*)tmpData.pointer)->type = TK_INT2FLOAT; 
        i++;
        ListInsert( (*listPostFix), x1.i+1 , tmpData ); // Vlozim Int2Float do listu PostFix vyrazu na pozici za prevadenou hodnotu

        tmpData.pointer = TokenInit();
        ((TToken*)tmpData.pointer)->type = TK_INT2FLOAT;
        i++;
        ListInsert( (*listPostFix), x2.i+2 , tmpData ); // Vlozim Int2Float do listu PostFix vyrazu na pozici za prevadenou hodnotu

        tmpData.pointer = NULL;
        tmpData.value = TK_NUM_DOUBLE;
        tmpData.i = i;
        ListPush( stack, tmpData );
        
      }
      /// Chyba kdyz: int + string, double + string, string + int, string + double
      else if( ( (x1.value == TK_NUM_DOUBLE || x1.value == TK_NUM_INTEGER) && x2.value == TK_NUM_STRING ) || ( x1.value == TK_NUM_STRING && (x2.value == TK_NUM_DOUBLE || x2.value == TK_NUM_INTEGER) ) ) {
        // ERR_COMP
        PrintLineErr( ((TToken*)data.pointer) );
        fprintf(stderr, "Nelze kombinovat datovy typ string s jinym datovym typem.\n" );
        CallError(ERR_COMP);
        break;
      }
      else if( x1.value == x2.value ) { // Stejne datove typy
        tmpData.value = ( IsOperatorCompare( ((TToken*)data.pointer)->type) )? TK_TRUE : x1.value;
        tmpData.i = i;
        ListPush( stack, tmpData );
      }
      else {
        // ERR_COMP
        PrintLineErr( ((TToken*)data.pointer) );
        fprintf(stderr, "Vzraz obsahuje neplatne informace.\n");
        CallError(ERR_COMP);
        break;
      }
    }
    else {
      // ?
    }
  }

  // printf(">>>semantic Expression\n"); /// DEBUG
  // for(i = 0; i < (*listPostFix)->count; i++ ) { /// DEBUG
  //   ListGet( (*listPostFix), i, &data ); /// DEBUG
  //   PrintToken( ((TToken*)data.pointer) ); /// DEBUG
  // }
  // printf("<<<sE\n"); /// DEBUG

  if( ListFront( stack, &tmpData) ) { // Ulozi se datovy typ vysledku
    (*nodeAST)->token2 = TokenInit();
    ((*nodeAST)->token2)->type = tmpData.value;
  }

  ListDestroy(stack);
  return 1;
}

//-----------------------------------------------
// Dodatecne funkce pro praci s Tabulkou Symbolu
//-----------------------------------------------

st_datatype_t TokenTypeToSymDataType(TTokenType type) {
  switch(type) {
    case TK_INTEGER:
    case TK_NUM_INTEGER:
      return SYM_DATATYPE_INT;
    case TK_DOUBLE:
    case TK_NUM_DOUBLE:
      return SYM_DATATYPE_DOUBLE;
    case TK_STRING:
    case TK_NUM_STRING:
      return SYM_DATATYPE_STRING;
    default: 
      return SYM_DATATYPE_ERROR;
  }
}
TTokenType SymDataTypeToTokenType(st_datatype_t type) {
  switch(type) {
    case SYM_DATATYPE_INT:
      return TK_NUM_INTEGER;

    case SYM_DATATYPE_DOUBLE:
      return TK_NUM_DOUBLE;

    case SYM_DATATYPE_STRING:
      return TK_NUM_STRING;

    default: 
      return TK_NA;
  }
}

//------------------------------------
// Vlozeni implicitnich funkci do tabulky symbolu
//------------------------------------

void SyntaxInitBuildInFunction() {
  symtable_elem_t *gel = NULL;
  TListData data;

  // length ( string ) as integer
  gel = AddElemGlobal(GlobalSymtable, "length");
  gel->elemType = SYM_TYPE_FUNCTION;
  gel->declared = 1;
  gel->defined = 1;
  gel->listParam = ListInit();
  data.value = SYM_DATATYPE_STRING;
  ListPushBack( gel->listParam, data );
  gel->dataType = SYM_DATATYPE_INT; 

  // substr ( string, integer, integer ) as string
  gel = AddElemGlobal(GlobalSymtable, "substr");
  gel->elemType = SYM_TYPE_FUNCTION;
  gel->declared = 1;
  gel->defined = 1;
  gel->listParam = ListInit();
  data.value = SYM_DATATYPE_STRING;
  ListPushBack( gel->listParam, data );
  data.value = SYM_DATATYPE_INT;
  ListPushBack( gel->listParam, data );
  data.value = SYM_DATATYPE_INT;
  ListPushBack( gel->listParam, data );
  gel->dataType = SYM_DATATYPE_STRING; 

  // asc ( string, integer ) as integer
  gel = AddElemGlobal(GlobalSymtable, "asc");
  gel->elemType = SYM_TYPE_FUNCTION;
  gel->declared = 1;
  gel->defined = 1;
  gel->listParam = ListInit();
  data.value = SYM_DATATYPE_STRING;
  ListPushBack( gel->listParam, data );
  data.value = SYM_DATATYPE_INT;
  ListPushBack( gel->listParam, data );
  gel->dataType = SYM_DATATYPE_INT; 

  // chr ( integer ) as string 
  gel = AddElemGlobal(GlobalSymtable, "chr");
  gel->elemType = SYM_TYPE_FUNCTION;
  gel->declared = 1;
  gel->defined = 1;
  gel->listParam = ListInit();
  data.value = SYM_DATATYPE_INT;
  ListPushBack( gel->listParam, data );
  gel->dataType = SYM_DATATYPE_STRING; 
}

#endif

// Soubor: src/parser2.c


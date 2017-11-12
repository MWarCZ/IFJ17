//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/parser.c                 //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef PARSER_C
#define PARSER_C

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "error.h"
#include "dstring.h"
#include "symtable.h"

symtable_t *GlobalSymtable; // Tabulka fuknci

TToken* GetNextDestroyOldToken(TToken *tkn, int canGetEOL) {
  do {
    TokenDestroy(tkn);
    tkn = GetNextToken();
  } while( !canGetEOL && tkn->type == TK_EOL );
  return tkn;
}

int Syntax_Program() {
  GlobalSymtable = SymtableInit();

  symtable_elem_t *gel = NULL;
  TToken* tkn = NULL;
  do {
    gel = NULL;
    tkn = GetNextDestroyOldToken(tkn,0);

    if(tkn->type == TK_DECLARE) { /// declare
      TokenDestroy(tkn);
      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_FUNCTION ) { /// declare function
        tkn = GetNextDestroyOldToken(tkn,1);
        if( tkn->type == TK_ID ) { /// declare function xyz
          // Najdi Funkci v tabulce symbolu pokud existuje
          gel = SymtableFind(GlobalSymtable, tkn->string );
          if(gel == NULL) { // Pokud neexistuje tak vytvor
            gel = AddElemGlobal(GlobalSymtable, tkn->string );
            gel->elemType = SYM_TYPE_FUNCTION;
          }
          if( gel->declared ) { // Funkce jiz byla jednout deklarovana
            // ERR_SYN
            CallError(ERR_SYN);
            return 0;
          }
          else { gel->declared = 1; }

          tkn = GetNextDestroyOldToken(tkn,1);
          if( tkn->type == TK_BRACKET_ROUND_LEFT ) { /// declare function xyz (
            tkn = GetNextDestroyOldToken(tkn,1);
            if( Syntax_ListParam(tkn, gel, 1) ) { /// declare function xyz ( ListParam 
              
            }
            else {
              // ERR_SYN
              CallError(ERR_SYN);
              return 0;
            }

            if( tkn->type == TK_BRACKET_ROUND_RIGHT ) { /// declare function xyz ( ListParam )
              tkn = GetNextDestroyOldToken(tkn,1);
            }
            else {
              // ERR_SYN
              CallError(ERR_SYN);
              return 0;
            }

            if( tkn->type == TK_AS ) { /// declare function xyz ( ListParam ) as 
              tkn = GetNextDestroyOldToken(tkn,1);
            }
            else {
              // ERR_SYN
              CallError(ERR_SYN);
              return 0;
            }

            if( tkn->type == TK_INTEGER ) { /// declare function xyz ( ListParam ) as DataType
              gel->dataType = SYM_DATATYPE_INT;
            }
            else if( tkn->type == TK_DOUBLE ) {
              gel->dataType = SYM_DATATYPE_DOUBLE;
            }
            else if( tkn->type == TK_STRING ) {
              gel->dataType = SYM_DATATYPE_STRING;
            }
            else {
              // ERR_SYN
              CallError(ERR_SYN);
              return 0;
            }
            tkn = GetNextDestroyOldToken(tkn,1);

            if( tkn->type == TK_EOL ) { /// declare function xyz ( ListParam ) as DataType EOL
              continue;
            }
            else {
              // ERR_SYN
              CallError(ERR_SYN);
              return 0;
            }
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            return 0;
          }
        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          return 0;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        return 0;
      }
    }
    //
    //---------------------------------------------------------------------------
    //
    else if( tkn->type == TK_FUNCTION ) { /// function
      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_ID ) { /// function xyz
        // Najdi Funkci v tabulce symbolu pokud existuje
        gel = SymtableFind(GlobalSymtable, tkn->string );
        if(gel == NULL) { // Pokud neexistuje tak vytvor
          gel = AddElemGlobal(GlobalSymtable, tkn->string );
          gel->elemType = SYM_TYPE_FUNCTION;
        }
        if( gel->defined ) { // Funkce jiz byla jednout definovana
          // ERR_SYN
          CallError(ERR_SYN);
          break;
        }
        else { gel->defined = 1; }

        tkn = GetNextDestroyOldToken(tkn,1);
        if( tkn->type == TK_BRACKET_ROUND_LEFT ) { /// function xyz (
          tkn = GetNextDestroyOldToken(tkn,1);
          if( Syntax_ListParam(tkn, gel, 0) ) { /// function xyz ( ListParam 
            
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            break;
          }

          if( tkn->type == TK_BRACKET_ROUND_RIGHT ) { /// function xyz ( ListParam )
            tkn = GetNextDestroyOldToken(tkn,1);
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            break;
          }

          if( tkn->type == TK_AS ) { /// function xyz ( ListParam ) as 
            tkn = GetNextDestroyOldToken(tkn,1);
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            break;
          }

          if( tkn->type == TK_INTEGER ) { /// function xyz ( ListParam ) as DataType
            gel->dataType = SYM_DATATYPE_INT;
          }
          else if( tkn->type == TK_DOUBLE ) {
            gel->dataType = SYM_DATATYPE_DOUBLE;
          }
          else if( tkn->type == TK_STRING ) {
            gel->dataType = SYM_DATATYPE_STRING;
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            break;
          }
          tkn = GetNextDestroyOldToken(tkn,1);

          if( tkn->type == TK_EOL ) { /// function xyz ( ListParam ) as DataType EOL
            // TODO - telo funkce
            tkn = GetNextDestroyOldToken(tkn,0);
            if( Syntax_FunctBody(tkn,gel) ) { /// function xyz ( ListParam ) as DataType EOL Funct_Body
            }
            else {
              // ERR_SYN
              CallError(ERR_SYN);
            }
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            break;
          }
          tkn = GetNextDestroyOldToken(tkn,1);

          if( tkn->type == TK_END ) { /// function xyz ( ListParam ) as DataType EOL Func_Body End
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            break;
          }
          tkn = GetNextDestroyOldToken(tkn,1);

          if( tkn->type == TK_FUNCTION ) { /// function xyz ( ListParam ) as DataType EOL Func_Body End Function
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            break;
          }
          tkn = GetNextDestroyOldToken(tkn,1);

          if( tkn->type == TK_EOL ) { /// function xyz ( ListParam ) as DataType EOL Func_Body End Function EOL
            continue;
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            break;
          }


        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          break;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

    }
    //
    //---------------------------------------------------------------------------
    //
    else if(tkn->type == TK_SCOPE) { /// scope
      // Token TK_ID s textem 'scope' se nemuze objevit
      // Najdi Funkci v tabulce symbolu pokud existuje - scope - pro jistotu
      gel = SymtableFind(GlobalSymtable, tkn->string ); 
      if(gel == NULL) { // Pokud neexistuje tak vytvor
        gel = AddElemGlobal(GlobalSymtable, tkn->string );
        gel->elemType = SYM_TYPE_FUNCTION;
      }

      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_EOL ) { /// scope EOL
        // TODO dodelat telo funkce
        tkn = GetNextDestroyOldToken(tkn,0);
        if( Syntax_FunctBody(tkn,gel) ) { /// scope EOL Func_Body
        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          break;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_END ) { /// scope EOL Func_Body end
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_SCOPE ) { /// scope EOL Func_Body end scope
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_EOL || tkn->type == TK_EOF ) { /// scope EOL Func_Body end scope EOL/EOF
        break;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

    }
    else {
      // ERR_SYN
      CallError(ERR_SYN);
      break;
    }

  } while(1);
  TokenDestroy(tkn);
  return !ERR_EXIT_STATUS;
} //- int Syntax_Program()

int Syntax_ListParam(TToken *tkn, symtable_elem_t *gel, int isDeclareNow) {
  symtable_elem_t *lel = NULL;
  int numParam = -1; // poradove cislo nacitaneho parametru - index

  while(1) {
    if(tkn->type == TK_ID) { /// xyz
      numParam++;
      if( !isDeclareNow ) { // Jedna se o Definici a do lokalni tabulky symbolu se vkladaji promene
        // Najdi nazev v globalni tabulce symbolu pokud existuje
        lel = SymtableFind( GlobalSymtable, tkn->string );
        if(lel != NULL) {
          // SYN_ERR - Promnena se jmenuje stejne jako jiz existujici funkce
          CallError(ERR_SYN);
          return 0;
        }
        // Najdi Promnenou v lokalni tabulce symbolu pokud existuje
        lel = SymtableFind( gel->local_symtable, tkn->string );
        if(lel == NULL) { // Pokud neexistuje tak vytvor
          lel = AddElemGlobal( gel->local_symtable, tkn->string );
          lel->elemType = SYM_TYPE_PARAM;
        }
        else {
          // SYN_ERR - Vice parametru se stejnym jmenem
          CallError(ERR_SYN);
          return 0;
        }
      }
      tkn = GetNextDestroyOldToken(tkn,1);
      if(tkn->type == TK_AS) { /// xyz as
        tkn = GetNextDestroyOldToken(tkn,1); /// xyz as DataType
        // TODO - Kontrola listu parametru/typu
        // if( list_DataType[numParam] == tkn->type ) else ERR
        if( !isDeclareNow ) {
          if( tkn->type == TK_INTEGER ) {
            lel->dataType = SYM_DATATYPE_INT;
          } 
          else if( tkn->type == TK_DOUBLE ) {
            lel->dataType = SYM_DATATYPE_DOUBLE;
          }
          else if( tkn->type == TK_STRING ) {
            lel->dataType = SYM_DATATYPE_STRING;
          }
          else {
            // SYN_ERR
            CallError(ERR_SYN);
            return 0;
          }

          tkn = GetNextDestroyOldToken(tkn,1);
          if( tkn->type == TK_COMMA ) { // Pokud je dalsi token ',' tak znovu opakuj.
            tkn = GetNextDestroyOldToken(tkn,1);
            continue;
          }
          return 1; // Pokud je jiny token tak skonci
        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          return 0;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        return 0;
      }
    }
    else { /// Prazdny list parametru
      return 2;
    }
  } //- while(1) 

} //- int Syntax_ListParam


int Syntax_FunctBody(TToken *tkn, symtable_elem_t *gel ) {
  if( Syntax_ListDefVar(tkn, gel) ) {

  }
  if( Syntax_ListCommand(tkn, gel) ) {

  } 
  
  return !ERR_EXIT_STATUS;
} //- int Syntax_FunctBody


int Syntax_ListDefVar(TToken *tkn, symtable_elem_t *gel ) {
  symtable_elem_t *lel = NULL;

  while(1) {
    lel = NULL;

    if( tkn->type == TK_DIM ) { /// dim
      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_ID ) { /// dim ID
        // Najdi nazev v globalni tabulce symbolu pokud existuje
        lel = SymtableFind( GlobalSymtable, tkn->string );
        if(lel != NULL) {
          // SYN_ERR - Promnena se jmenuje stejne jako jiz existujici funkce
          CallError(ERR_SYN);
          return 0;
        }
        // Najdi Promnenou v lokalni tabulce symbolu pokud existuje
        lel = SymtableFind( gel->local_symtable, tkn->string );
        if(lel == NULL) { // Pokud neexistuje tak vytvor
          lel = AddElemGlobal( gel->local_symtable, tkn->string );
          lel->elemType = SYM_TYPE_VAR;
        }
        else {
          // SYN_ERR - Vice parametru/promnenych se stejnym jmenem
          CallError(ERR_SYN);
          break;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }
    
      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_AS ) { /// dim ID as
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }
    
      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_INTEGER ) { /// dim ID as DataType
        lel->dataType = SYM_DATATYPE_INT;
      }
      else if( tkn->type == TK_DOUBLE ) { 
        lel->dataType = SYM_DATATYPE_DOUBLE;
      }
      else if( tkn->type == TK_STRING ) { 
        lel->dataType = SYM_DATATYPE_STRING;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_EOL ) { /// dim ID as DataType EOL
        tkn = GetNextDestroyOldToken(tkn,0); // Konec nalezeneho proto dalsi token bude ignorovat prazdne radky
        continue;
      }
      else if( tkn->type == TK_EQUAL ) { /// dim ID as DataType = 
        if( /*Expresion()*/ false ) { /// dim ID as DataType = Expresion
          // TODO - Expression - zpracovani vyrazu
        
        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          break;
        }

        tkn = GetNextDestroyOldToken(tkn,1);
        if( tkn->type == TK_EOL ) { /// dim ID as DataType = Expresion EOL
          tkn = GetNextDestroyOldToken(tkn,0); // Konec nalezeneho proto dalsi token bude ignorovat prazdne radky
          continue;
        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          break;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }
    } //- if( tkn->type == TK_DIM ) 
    else {
      /// Prazdno - bez definic promenych
      break;
    }


  } //- while(1) 

  return !ERR_EXIT_STATUS;
} //- int Syntax_ListDefVar

int Syntax_ListCommand(TToken *tkn, symtable_elem_t *gel ) {
  symtable_elem_t *lel = NULL;

  while(1) {
    lel = NULL;

    if( tkn->type == TK_INPUT ) { /// input
      tkn = GetNextDestroyOldToken(tkn,1);

      if( tkn->type == TK_ID ) { /// input ID

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }
      tkn = GetNextDestroyOldToken(tkn,1);

      if( tkn->type == TK_EOF ) { /// input ID EOF
        tkn = GetNextDestroyOldToken(tkn,0);
        continue;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

    } //- if( tkn->type == TK_INPUT )
    //
    //----------------------------------------------------------
    //
    else if( tkn->type == TK_PRINT ) { /// print
      tkn = GetNextDestroyOldToken(tkn,1);
      if( ListExpression() ) { /// print ListExpression
        // TODO - ListExpression - Expresion; Expresion;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_EOL ) { /// print ListExpression EOL
        tkn = GetNextDestroyOldToken(tkn,0);
        continue;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

    } //- else if( tkn->type == TK_PRINT )
    //
    //----------------------------------------------------------
    //
    else if( tkn->type == TK_ID ) { /// ID
      // Najdi nazev v globalni tabulce symbolu pokud existuje
      lel = SymtableFind( GlobalSymtable, tkn->string );
      if(lel != NULL) { 
        // ERR_SYN - ID je nazev funkce - Do funkce nelze priradit hodnotu.
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_EQUAL ) { /// ID = 

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_ID ) {
        // Najdi nazev v globalni tabulce symbolu pokud existuje
        lel = SymtableFind( GlobalSymtable, tkn->string );
        if(lel != NULL) { /// ID = ID
          // ID je funkce
          tkn = GetNextDestroyOldToken(tkn,1);
          if( tkn->type == TK_BRACKET_ROUND_LEFT ) { /// ID = ID (

          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            break;
          }

          if( ListInParam() ) { /// ID = ID ( ListInParam
            // TODO - ListInParam
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            break;
          }

          tkn = GetNextDestroyOldToken(tkn,1);
          if( tkn->type == TK_BRACKET_ROUND_LEFT ) { /// ID = ID ( ListParam )

          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            break;
          }

          tkn = GetNextDestroyOldToken(tkn,1);
          if( tkn->type == TK_EOL ) { /// ID = ID ( ListParam ) EOL
            tkn = GetNextDestroyOldToken(tkn,0);
            continue;
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            break;
          }
        }
        else {
          if ( Expression() ) { /// ID = Expression
            // TODO - Expression
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            break;
          }

          //tkn = GetNextDestroyOldToken(tkn,1); // Mel bych dostat token z Expression()
          if( tkn->type == TK_EOL ) { /// ID = Expression EOL
            tkn = GetNextDestroyOldToken(tkn,0);
            continue;
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            break;
          }
        }
      }
      else if( Expression() ) {
        if ( Expression() ) { /// ID = Expression
          // TODO - Expression
        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          break;
        }

        if( tkn->type == TK_EOL ) { /// ID = Expression EOL
          tkn = GetNextDestroyOldToken(tkn,0);
          continue;
        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          break;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

    } //- else if( tkn->type == TK_ID )
    //
    //----------------------------------------------------------
    //
    else if( tkn->type == TK_RETURN ) { /// return
      tkn = GetNextDestroyOldToken(tkn,1);

      if ( Expression() ) { /// return Expression
        // TODO - Expression
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      if( tkn->type == TK_EOL ) { /// return Expression EOL
        tkn = GetNextDestroyOldToken(tkn,0);
        continue;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

    } //- else if( tkn->type == TK_RETURN ) 
    //
    //----------------------------------------------------------
    //
    else if( tkn->type == TK_IF ) { /// if
      tkn = GetNextDestroyOldToken(tkn,1);

      if( Syntax_Condition(tkn, gel) ) { /// if Assignment

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      if( tkn->type == TK_THEN ) { /// if Assignment then
        tkn = GetNextDestroyOldToken(tkn,0);
        continue;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_EOL ) { /// if Assignment then EOL
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,0);
      // TODO - ListCommand
      if( Syntax_ListCommand(tkn, gel ) ) { /// if Assignment then EOL ListCommand

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      if( tkn->type == TK_ELSE) { /// if Assignment then EOL ListCommand else

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_EOL) { /// if Assignment then EOL ListCommand else EOL

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,0);
      // TODO - ListCommand
      if( Syntax_ListCommand(tkn, gel ) ) { /// if Assignment then EOL ListCommand else EOL ListCommand

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      if( tkn->type == TK_END) { /// if Assignment then EOL ListCommand else EOL ListCommand end

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_IF) { /// if Assignment then EOL ListCommand else EOL ListCommand end if

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_EOL) { /// if Assignment then EOL ListCommand else EOL ListCommand end if EOL
        tkn = GetNextDestroyOldToken(tkn,0);
        continue;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

    } //- else if( tkn->type == TK_IF )
    //
    //----------------------------------------------------------
    //
    else if( tkn->type == TK_DO ) { /// do
      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_WHILE) { /// do while

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,1);
      if( Syntax_Condition(tkn, gel) ) { /// do while Assignment

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      if( tkn->type == TK_EOL) { /// do while Assignment EOL

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,0);
      // TODO - ListCommand
      if( Syntax_ListCommand(tkn, gel ) ) { /// do while Assignment EOL ListCommand

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      if( tkn->type == TK_EOL) { /// do while Assignment EOL ListCommand loop

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

      tkn = GetNextDestroyOldToken(tkn,1);
      if( tkn->type == TK_EOL) { /// do while Assignment EOL ListCommand loop EOL
        tkn = GetNextDestroyOldToken(tkn,0);
        continue;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        break;
      }

    } //- else if( tkn->type == TK_DO )
    //
    //----------------------------------------------------------
    //
    else {
      /// Prazdne telo funkce
      break;
    }

  } //- while(1) 

  return !ERR_EXIT_STATUS;
} //- int Syntax_ListCommand


int Syntax_Condition(TToken *tkn, symtable_elem_t *gel ) {
  if ( Expression() ) { /// if Expression
    // TODO - Expression
    // TODO - < > <> = >= <=
    if( Syntax_RelationalOperator(tkn, gel) ) {

      tkn = GetNextDestroyOldToken(tkn,1);

      if ( Expression() ) { /// if Expression RO Expression
        // TODO - Expression
      } 
      else {
        // ERR_SYN
        CallError(ERR_SYN);
      }
    } 
    else {
      // ERR_SYN
      CallError(ERR_SYN);
    }
  }
  else {
        // ERR_SYN
    CallError(ERR_SYN);
  }

  return !ERR_EXIT_STATUS;
} //- int Syntax_Condition


int Syntax_RelationalOperator(TToken *tkn, symtable_elem_t *gel ) {
  if( tkn->type == TK_EQUAL ) {
  }
  else if( tkn->type == TK_NOT_EQUAL ) {
  }
  else if( tkn->type == TK_GREATER ) {
  }
  else if( tkn->type == TK_GREATER_EQUAL ) {
  }
  else if( tkn->type == TK_LESS ) {
  }
  else if( tkn->type == TK_LESS_EQUAL ) {
  }
  else {
    CallError(ERR_SYN);
  }
  return !ERR_EXIT_STATUS;
} //- int Syntax_RelationalOperator


int Syntax_Expression(TToken *tkn, symtable_elem_t *gel ) {

} //- int Syntax_Expression

#endif

// Soubor: src/parser.c


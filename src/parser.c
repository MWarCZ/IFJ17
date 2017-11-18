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
#include <string.h>
#include "parser.h"
#include "scanner.h"
#include "token.h"
#include "error.h"
#include "dstring.h"
#include "symtable.h"
#include "list.h"

symtable_t *GlobalSymtable; // Globalni tabulka symbolu alias Tabulka fuknci

// Znici stary token a vrati novy
TToken* GetNextDestroyOldToken(TToken *tkn, int canGetEOL) {
  do {
    TokenDestroy( tkn );
    tkn = GetNextToken();
  } while( !canGetEOL &&  tkn->type == TK_EOL );
  return tkn;
}

int SyntaxStartParse() {
  SymtableInit(&GlobalSymtable);
  TToken *tmpToken = NULL;
  TToken** tkn = &tmpToken;

  if( Syntax_Program(tkn) ) {
  }
  else {
    // ERR_SYN
    CallError(ERR_SYN);
  }

  TokenDestroy( (*tkn) );
  SymtableFree(GlobalSymtable);

  return !ERR_EXIT_STATUS;
}

int Syntax_Program(TToken **tkn) {

  int existScope = 0; // Jiz byl scope vytvoren?
  symtable_elem_t *gel = NULL;

  do {
    gel = NULL;
    (*tkn) = GetNextDestroyOldToken( (*tkn),0);

    if(  (*tkn)->type == TK_EOF) {
      break;
    }
    else if( existScope ) {
      // ERR_SYN
      CallError(ERR_SYN);
      fprintf(stderr, ">Po funkci scope jiz neni mozne dale pokracovat.\n");
      break;
    }
    else if( (*tkn)->type == TK_DECLARE) { /// declare
      
      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_FUNCTION ) { /// declare function
        (*tkn) = GetNextDestroyOldToken( (*tkn),1);
        if(  (*tkn)->type == TK_ID ) { /// declare function xyz
          // Najdi Funkci v tabulce symbolu pokud existuje
          gel = SymtableFind(GlobalSymtable,  (*tkn)->string );
          if(gel == NULL) { // Pokud neexistuje tak vytvor
            gel = AddElemGlobal(GlobalSymtable,  (*tkn)->string );
            gel->elemType = SYM_TYPE_FUNCTION;
          }
          if( gel->declared ) { // Funkce jiz byla jednout deklarovana
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">declare function xyz - jiz existuje\n");
            return 0;
          }
          else { gel->declared = 1; }

          (*tkn) = GetNextDestroyOldToken( (*tkn),1);
          if(  (*tkn)->type == TK_BRACKET_ROUND_LEFT ) { /// declare function xyz (
            (*tkn) = GetNextDestroyOldToken( (*tkn),1);
            if( Syntax_ListParam(tkn, gel, 1) ) { /// declare function xyz ( ListParam 
              
            }
            else {
              // ERR_SYN
              CallError(ERR_SYN);
              fprintf(stderr, ">declare function xyz ( ListParam\n");
              return 0;
            }

            if(  (*tkn)->type == TK_BRACKET_ROUND_RIGHT ) { /// declare function xyz ( ListParam )
              (*tkn) = GetNextDestroyOldToken( (*tkn),1);
            }
            else {
              // ERR_SYN
              CallError(ERR_SYN);
              fprintf(stderr, ">declare function xyz ( ListParam )\n");
              return 0;
            }

            if(  (*tkn)->type == TK_AS ) { /// declare function xyz ( ListParam ) as 
              (*tkn) = GetNextDestroyOldToken( (*tkn),1);
            }
            else {
              // ERR_SYN
              CallError(ERR_SYN);
              fprintf(stderr, ">declare function xyz ( ListParam ) as\n");
              return 0;
            }

            unsigned int symDataType;
            if( (*tkn)->type == TK_INTEGER || (*tkn)->type == TK_DOUBLE || (*tkn)->type == TK_STRING ) { /// declare function xyz ( ListParam ) as DataType
              switch( (*tkn)->type ) {
                case TK_INTEGER: symDataType = SYM_DATATYPE_INT; break;
                case TK_DOUBLE: symDataType = SYM_DATATYPE_DOUBLE; break;
                case TK_STRING: symDataType = SYM_DATATYPE_STRING; break;
                default: break;
              }
            }
            else {
              // ERR_SYN
              CallError(ERR_SYN);
              fprintf(stderr, ">declare function xyz ( ListParam ) as DataType\n");
              return 0;
            }
            if( gel->defined ) {
              if (gel->dataType != symDataType) {
                // ERR_SYN
                CallError(ERR_SYN);
                fprintf(stderr, ">Neschoduji se navratove datove typy definice a declarace funkce.\n");
                return 0;
              }
            }
            else {
              gel->dataType = symDataType;
            }

            (*tkn) = GetNextDestroyOldToken( (*tkn),1);

            if(  (*tkn)->type == TK_EOL ) { /// declare function xyz ( ListParam ) as DataType EOL
              continue;
            }
            else {
              // ERR_SYN
              CallError(ERR_SYN);
              fprintf(stderr, ">declare function xyz ( ListParam ) as DataType EOL\n");
              return 0;
            }
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">declare function xyz (\n");
            return 0;
          }
        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          fprintf(stderr, ">declare function xyz\n");
          return 0;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">declare function\n");
        return 0;
      }
    }
    //
    //---------------------------------------------------------------------------
    //
    else if(  (*tkn)->type == TK_FUNCTION ) { /// function
      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_ID ) { /// function xyz
        // Najdi Funkci v tabulce symbolu pokud existuje
        gel = SymtableFind(GlobalSymtable,  (*tkn)->string );
        if(gel == NULL) { // Pokud neexistuje tak vytvor
          gel = AddElemGlobal(GlobalSymtable,  (*tkn)->string );
          gel->elemType = SYM_TYPE_FUNCTION;
        }
        if( gel->defined ) { // Funkce jiz byla jednout definovana
          // ERR_SYN
          CallError(ERR_SYN);
          fprintf(stderr, ">function xyz - Funkce jiz byla jednou definovana.\n");
          break;
        }
        else { gel->defined = 1; }

        (*tkn) = GetNextDestroyOldToken( (*tkn),1);
        if(  (*tkn)->type == TK_BRACKET_ROUND_LEFT ) { /// function xyz (
          (*tkn) = GetNextDestroyOldToken( (*tkn),1);
          if( Syntax_ListParam(tkn, gel, 0) ) { /// function xyz ( ListParam 
            
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">function xyz ( ListParam\n");
            break;
          }

          if(  (*tkn)->type == TK_BRACKET_ROUND_RIGHT ) { /// function xyz ( ListParam )
            (*tkn) = GetNextDestroyOldToken( (*tkn),1);
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">function xyz ( ListParam )\n");
            break;
          }

          if(  (*tkn)->type == TK_AS ) { /// function xyz ( ListParam ) as 
            (*tkn) = GetNextDestroyOldToken( (*tkn),1);
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">function xyz ( ListParam ) as\n");
            break;
          }

          /*
          if(  (*tkn)->type == TK_INTEGER ) { /// function xyz ( ListParam ) as DataType
            gel->dataType = SYM_DATATYPE_INT;
          }
          else if(  (*tkn)->type == TK_DOUBLE ) {
            gel->dataType = SYM_DATATYPE_DOUBLE;
          }
          else if(  (*tkn)->type == TK_STRING ) {
            gel->dataType = SYM_DATATYPE_STRING;
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">function xyz ( ListParam ) as DataType\n");
            break;
          }*/

          unsigned int symDataType = 0;
          if( (*tkn)->type == TK_INTEGER || (*tkn)->type == TK_DOUBLE || (*tkn)->type == TK_STRING ) { /// function xyz ( ListParam ) as DataType
            switch( (*tkn)->type ) {
              case TK_INTEGER: symDataType = SYM_DATATYPE_INT; break;
              case TK_DOUBLE: symDataType = SYM_DATATYPE_DOUBLE; break;
              case TK_STRING: symDataType = SYM_DATATYPE_STRING; break;
              default: break;
            }
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">function xyz ( ListParam ) as DataType\n");
            return 0;
          }
          if( gel->declared ) {
            if (gel->dataType != symDataType) {
              // ERR_SYN
              CallError(ERR_SYN);
              fprintf(stderr, ">Neschoduji se navratove datove typy definice a declarace funkce.\n");
              return 0;
            }
          }
          else {
            gel->dataType = symDataType;
          }
            //------

          (*tkn) = GetNextDestroyOldToken( (*tkn),1);

          if(  (*tkn)->type == TK_EOL ) { /// function xyz ( ListParam ) as DataType EOL
            // TODO - telo funkce
            (*tkn) = GetNextDestroyOldToken( (*tkn),0);
            if( Syntax_FunctBody(tkn,gel) ) { /// function xyz ( ListParam ) as DataType EOL Funct_Body
            }
            else {
              // ERR_SYN
              CallError(ERR_SYN);
              fprintf(stderr, ">function xyz ( ListParam ) as DataType EOL Funct_Body\n");
              break;
            }
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">function xyz ( ListParam ) as DataType EOL\n");
            break;
          }
          //(*tkn) = GetNextDestroyOldToken( (*tkn),1);

          if(  (*tkn)->type == TK_END ) { /// function xyz ( ListParam ) as DataType EOL Func_Body End
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">function xyz ( ListParam ) as DataType EOL Funct_Body end\n");
            break;
          }
          (*tkn) = GetNextDestroyOldToken( (*tkn),1);

          if(  (*tkn)->type == TK_FUNCTION ) { /// function xyz ( ListParam ) as DataType EOL Func_Body End Function
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">function xyz ( ListParam ) as DataType EOL Funct_Body end function\n");
            break;
          }
          (*tkn) = GetNextDestroyOldToken( (*tkn),1);

          if(  (*tkn)->type == TK_EOL ) { /// function xyz ( ListParam ) as DataType EOL Func_Body End Function EOL
            continue;
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">function xyz ( ListParam ) as DataType EOL Funct_Body end function EOL\n");
            break;
          }


        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          fprintf(stderr, ">function xyz (\n");
          break;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">function xyz\n");
        break;
      }

    }
    //
    //---------------------------------------------------------------------------
    //
    else if( (*tkn)->type == TK_SCOPE) { /// scope
      existScope = 1;
      // Token TK_ID s textem 'scope' se nemuze objevit
      // Najdi Funkci v tabulce symbolu pokud existuje - scope - pro jistotu
      gel = SymtableFind(GlobalSymtable,  (*tkn)->string ); 
      if(gel == NULL) { // Pokud neexistuje tak vytvor
        gel = AddElemGlobal(GlobalSymtable,  (*tkn)->string );
        gel->elemType = SYM_TYPE_FUNCTION;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">Hlavni funkce scope jiz existuje.\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_EOL ) { /// scope EOL
        // TODO dodelat telo funkce
        (*tkn) = GetNextDestroyOldToken( (*tkn),0);
        if( Syntax_FunctBody(tkn,gel) ) { /// scope EOL Func_Body
        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          fprintf(stderr, ">scope EOL Func_Body\n");
          break;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">scope EOL\n");
        break;
      }

      //(*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_END ) { /// scope EOL Func_Body end
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">scope EOL Func_Body end\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_SCOPE ) { /// scope EOL Func_Body end scope
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">scope EOL Func_Body end scope\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_EOL ||  (*tkn)->type == TK_EOF ) { /// scope EOL Func_Body end scope EOL/EOF
        //break;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">scope EOL Func_Body end scope EOL/EOF\n");
        break;
      }

    }
    else {
      // ERR_SYN
      CallError(ERR_SYN);
      fprintf(stderr, ">UNOWN\n");
      break;
    }

  } while(1);


  return !ERR_EXIT_STATUS && existScope;
} //- int Syntax_Program()


int Syntax_FunctBody(TToken **tkn, symtable_elem_t *gel ) {
  if( Syntax_ListDefVar(tkn, gel) ) {

  }
  if( Syntax_ListCommand(tkn, gel) ) {

  } 
  
  return !ERR_EXIT_STATUS;
} //- int Syntax_FunctBody


int Syntax_ListDefVar(TToken **tkn, symtable_elem_t *gel ) {
  symtable_elem_t *lel = NULL;

  while(1) {
    lel = NULL;

    if( (*tkn)->type == TK_DIM ) { /// dim
      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if( (*tkn)->type == TK_ID ) { /// dim ID
        // Najdi nazev v globalni tabulce symbolu pokud existuje
        lel = SymtableFind( GlobalSymtable, (*tkn)->string );
        if(lel != NULL) {
          // ERR_SYN - Promnena se jmenuje stejne jako jiz existujici funkce
          CallError(ERR_SYN);
          fprintf(stderr, ">dim ID - Stejne jmeno jako existujici funkce.\n");
          return 0;
        }
        // Najdi Promnenou v lokalni tabulce symbolu pokud existuje
        lel = SymtableFind( gel->local_symtable, (*tkn)->string );
        if(lel == NULL) { // Pokud neexistuje tak vytvor
          lel = AddElemGlobal( gel->local_symtable, (*tkn)->string );
          lel->elemType = SYM_TYPE_VAR;
        }
        else {
          // ERR_SYN - Vice parametru/promnenych se stejnym jmenem
          CallError(ERR_SYN);
          fprintf(stderr, ">dim ID - Promena se jmenuje stejne jako jina promena/parametr funkce.\n");
          break;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">dim ID\n");
        break;
      }
    
      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_AS ) { /// dim ID as
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">dim ID as\n");
        break;
      }
    
      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if( (*tkn)->type == TK_INTEGER ) { /// dim ID as DataType
        lel->dataType = SYM_DATATYPE_INT;
      }
      else if( (*tkn)->type == TK_DOUBLE ) { 
        lel->dataType = SYM_DATATYPE_DOUBLE;
      }
      else if( (*tkn)->type == TK_STRING ) { 
        lel->dataType = SYM_DATATYPE_STRING;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">dim ID as DataType\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if( (*tkn)->type == TK_EOL ) { /// dim ID as DataType EOL
        (*tkn) = GetNextDestroyOldToken( (*tkn),0); // Konec nalezeneho proto dalsi token bude ignorovat prazdne radky
        continue;
      }
      else if( (*tkn)->type == TK_EQUAL ) { /// dim ID as DataType = 
        (*tkn) = GetNextDestroyOldToken( (*tkn),1);
        if( Syntax_Expression(tkn, gel) ) { /// dim ID as DataType = Expresion
          // TODO - Expression - zpracovani vyrazu
        
        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          fprintf(stderr, ">dim ID as DataType = Expression\n");
          break;
        }

        //(*tkn) = GetNextDestroyOldToken( (*tkn),1);
        if( (*tkn)->type == TK_EOL ) { /// dim ID as DataType = Expresion EOL
          (*tkn) = GetNextDestroyOldToken( (*tkn),0); // Konec nalezeneho proto dalsi token bude ignorovat prazdne radky
          continue;
        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          fprintf(stderr, ">dim ID as DataType = Expression EOL\n");
          break;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">dim ID as DataType =/EOL\n");
        break;
      }
    } //- if(  (*tkn)->type == TK_DIM ) 
    else {
      /// Prazdno - bez definic promenych
      break;
    }


  } //- while(1) 

  return !ERR_EXIT_STATUS;
} //- int Syntax_ListDefVar

int Syntax_ListCommand(TToken **tkn, symtable_elem_t *gel ) {
  symtable_elem_t *lel = NULL;

  while(1) {
    lel = NULL;

    if( (*tkn)->type == TK_INPUT ) { /// input
      (*tkn) = GetNextDestroyOldToken( (*tkn),1);

      if( (*tkn)->type == TK_ID ) { /// input ID
        lel = SymtableFind( gel->local_symtable, (*tkn)->string );
        if(lel == NULL) {
          // ERR_SYN
          CallError(ERR_SYN);
          fprintf(stderr, ">input ID - Promena ID neexistuje\n");
          break;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">input ID\n");
        break;
      }
      (*tkn) = GetNextDestroyOldToken( (*tkn),1);

      if(  (*tkn)->type == TK_EOL ) { /// input ID EOL
        (*tkn) = GetNextDestroyOldToken( (*tkn),0);
        continue;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">input ID EOL\n");
        break;
      }

    } //- if(  (*tkn)->type == TK_INPUT )
    //
    //----------------------------------------------------------
    //
    else if(  (*tkn)->type == TK_PRINT ) { /// print
      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if( Syntax_ListExpression(tkn, gel) ) { /// print ListExpression
        
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">print ListExpression\n");
        break;
      }

      //(*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_EOL ) { /// print ListExpression EOL
        (*tkn) = GetNextDestroyOldToken( (*tkn),0);
        continue;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">print ListExpression EOL\n");
        break;
      }

    } //- else if(  (*tkn)->type == TK_PRINT )
    //
    //----------------------------------------------------------
    //
    else if(  (*tkn)->type == TK_ID ) { /// ID
      // Najdi nazev v globalni tabulce symbolu pokud existuje
      lel = SymtableFind( GlobalSymtable,  (*tkn)->string );
      if(lel != NULL) { 
        // ERR_SYN - ID je nazev funkce - Do funkce nelze priradit hodnotu.
        CallError(ERR_SYN);
        fprintf(stderr, ">ID - ID je nazev funkce. Do funkce nelze priradit hodnotu.\n");
        break;
      }
      // Najdi nazev v lokalni tabulce symbolu pokud existuje
      lel = SymtableFind( gel->local_symtable,  (*tkn)->string );
      if(lel == NULL) { 
        // ERR_SYN - Promena s nazvem ID neexistuje
        CallError(ERR_SYN);
        fprintf(stderr, ">ID - ID neni nazev existujici promene.\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_EQUAL ) { /// ID = 

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">ID =\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_ID ) {
        // Najdi nazev v globalni tabulce symbolu pokud existuje
        lel = SymtableFind( GlobalSymtable,  (*tkn)->string );
        if(lel != NULL) { /// ID = ID
          // ID je funkce
          (*tkn) = GetNextDestroyOldToken( (*tkn),1);
          if(  (*tkn)->type == TK_BRACKET_ROUND_LEFT ) { /// ID = ID (

          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">ID = ID (\n");
            break;
          }

          if( /*ListInParam()*/ false ) { /// ID = ID ( ListInParam
            // TODO - ListInParam
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">ID = ID ( ListInParam\n");
            break;
          }

          (*tkn) = GetNextDestroyOldToken( (*tkn),1);
          if(  (*tkn)->type == TK_BRACKET_ROUND_LEFT ) { /// ID = ID ( ListInParam )

          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">ID = ID ( ListInParam )\n");
            break;
          }

          (*tkn) = GetNextDestroyOldToken( (*tkn),1);
          if(  (*tkn)->type == TK_EOL ) { /// ID = ID ( ListInParam ) EOL
            (*tkn) = GetNextDestroyOldToken( (*tkn),0);
            continue;
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">ID = ID ( ListInParam ) EOL\n");
            break;
          }
        }
        else {
          if ( Syntax_Expression(tkn, gel) ) { /// ID = Expression
            // TODO - Expression
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">ID = Expression1\n");
            break;
          }

          //(*tkn) = GetNextDestroyOldToken( (*tkn),1); // Mel bych dostat token z Expression()
          if(  (*tkn)->type == TK_EOL ) { /// ID = Expression EOL
            (*tkn) = GetNextDestroyOldToken( (*tkn),0);
            continue;
          }
          else {
            // ERR_SYN
            CallError(ERR_SYN);
            fprintf(stderr, ">ID = Expression1 EOL\n");
            break;
          }
        }
      } //- else if(  (*tkn)->type == TK_ID )
      else if(  (*tkn)->type == TK_NUM_INTEGER ||  (*tkn)->type == TK_NUM_DOUBLE ||  (*tkn)->type == TK_NUM_STRING ||  (*tkn)->type == TK_BRACKET_ROUND_LEFT ) {
        if ( Syntax_Expression(tkn, gel) ) { /// ID = Expression
          // TODO - Expression
        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          fprintf(stderr, ">ID = Expression\n");
          break;
        }

        if(  (*tkn)->type == TK_EOL ) { /// ID = Expression EOL
          (*tkn) = GetNextDestroyOldToken( (*tkn),0);
          continue;
        }
        else {
          // ERR_SYN
          CallError(ERR_SYN);
          fprintf(stderr, ">ID = Expression EOL\n");
          break;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">ID = - bylo ocekavano ID funkce nebo Vyraz\n");
        break;
      }

    } 
    //
    //----------------------------------------------------------
    //
    else if(  (*tkn)->type == TK_RETURN ) { /// return

      if ( strcmp("scope",gel->name)==0 ) { // telo scope nesmi obsahovat return
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">return se nesmi vyskytovat v tele scope\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);

      if ( Syntax_Expression(tkn, gel) ) { /// return Expression
        // TODO - Expression
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">return Expression\n");
        break;
      }

      if(  (*tkn)->type == TK_EOL ) { /// return Expression EOL
        (*tkn) = GetNextDestroyOldToken( (*tkn),0);
        continue;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">return Expression EOL\n");
        break;
      }

    } //- else if(  (*tkn)->type == TK_RETURN ) 
    //
    //----------------------------------------------------------
    //
    else if(  (*tkn)->type == TK_IF ) { /// if
      (*tkn) = GetNextDestroyOldToken( (*tkn),1);

      if( Syntax_Condition(tkn, gel) ) { /// if Assignment

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">if Assignment\n");
        break;
      }

      if(  (*tkn)->type == TK_THEN ) { /// if Assignment then
        // (*tkn) = GetNextDestroyOldToken( (*tkn),0);
        // continue;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">if Assignment then\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_EOL ) { /// if Assignment then EOL
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">if Assignment then EOL\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),0);
      // TODO - ListCommand
      if( Syntax_ListCommand(tkn, gel ) ) { /// if Assignment then EOL ListCommand

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">if Assignment then EOL ListCommand\n");
        break;
      }

      if(  (*tkn)->type == TK_ELSE) { /// if Assignment then EOL ListCommand else

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">if Assignment then EOL ListCommand else\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_EOL) { /// if Assignment then EOL ListCommand else EOL

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">if Assignment then EOL ListCommand else EOL\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),0);
      // TODO - ListCommand
      if( Syntax_ListCommand(tkn, gel ) ) { /// if Assignment then EOL ListCommand else EOL ListCommand

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">if Assignment then EOL ListCommand else EOL ListCommand\n");
        break;
      }

      if(  (*tkn)->type == TK_END) { /// if Assignment then EOL ListCommand else EOL ListCommand end

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">if Assignment then EOL ListCommand else EOL ListCommand end\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_IF) { /// if Assignment then EOL ListCommand else EOL ListCommand end if

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">if Assignment then EOL ListCommand else EOL ListCommand end if\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_EOL) { /// if Assignment then EOL ListCommand else EOL ListCommand end if EOL
        (*tkn) = GetNextDestroyOldToken( (*tkn),0);
        continue;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">if Assignment then EOL ListCommand else EOL ListCommand end if EOL\n");
        break;
      }

    } //- else if(  (*tkn)->type == TK_IF )
    //
    //----------------------------------------------------------
    //
    else if(  (*tkn)->type == TK_DO ) { /// do
      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_WHILE) { /// do while

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">do while\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if( Syntax_Condition(tkn, gel) ) { /// do while Assignment

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">do while Assignment\n");
        break;
      }

      if(  (*tkn)->type == TK_EOL) { /// do while Assignment EOL

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">do while Assignment EOL\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),0);
      // TODO - ListCommand
      if( Syntax_ListCommand(tkn, gel ) ) { /// do while Assignment EOL ListCommand

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">do while Assignment EOL ListCommand\n");
        break;
      }

      if(  (*tkn)->type == TK_LOOP) { /// do while Assignment EOL ListCommand loop

      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">do while Assignment EOL ListCommand loop\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if(  (*tkn)->type == TK_EOL) { /// do while Assignment EOL ListCommand loop EOL
        (*tkn) = GetNextDestroyOldToken( (*tkn),0);
        continue;
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">do while Assignment EOL ListCommand loop EOL\n");
        break;
      }

    } //- else if(  (*tkn)->type == TK_DO )
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


int Syntax_Condition(TToken **tkn, symtable_elem_t *gel ) {
  (void)gel; // unused warning
  if ( Syntax_Expression(tkn, gel) ) { /// Expression
    // TODO - Expression
    // TODO - < > <> = >= <=
    if( Syntax_RelationalOperator(tkn, gel) ) { /// Expression RO

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);

      if ( Syntax_Expression(tkn, gel) ) { /// Expression RO Expression
        // TODO - Expression
      } 
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">Expression RO Expression\n");
      }
    } 
    else {
      // ERR_SYN
      CallError(ERR_SYN);
      fprintf(stderr, ">Expression RO\n");
    }
  }
  else {
        // ERR_SYN
    CallError(ERR_SYN);
    fprintf(stderr, ">Expression\n");
  }

  return !ERR_EXIT_STATUS;
} //- int Syntax_Condition


int Syntax_RelationalOperator(TToken **tkn, symtable_elem_t *gel ) {
  (void)gel; // unused parametr
  if(  (*tkn)->type == TK_EQUAL ) {
  }
  else if(  (*tkn)->type == TK_NOT_EQUAL ) {
  }
  else if(  (*tkn)->type == TK_GREATER ) {
  }
  else if(  (*tkn)->type == TK_GREATER_EQUAL ) {
  }
  else if(  (*tkn)->type == TK_LESS ) {
  }
  else if(  (*tkn)->type == TK_LESS_EQUAL ) {
  }
  else {
    CallError(ERR_SYN);
    fprintf(stderr, ">RO\n");
  }
  return !ERR_EXIT_STATUS;
} //- int Syntax_RelationalOperator



//----------------------------------------
// Syntax_Expression
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
      return 4;
    case TK_PLUS:
    case TK_MINUS:
      return 3;
    case TK_MUL:
    case TK_DIV:
      return 1;
    case TK_MOD:
    case TK_DIV_INT:
      return 2;
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
        case TK_MOD:
        case TK_DIV_INT:
        case TK_BRACKET_ROUND_RIGHT:
          return 1;
        default:
          return 0;
      }
    case TK_BRACKET_ROUND_LEFT:
    case TK_PLUS:
    case TK_MINUS:
    case TK_MUL:
    case TK_DIV:
    case TK_MOD:
    case TK_DIV_INT:
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
    case TK_MOD:
    case TK_DIV_INT:
      return 1;
      break;
    default:
      return 0;
      break;
  }

}

int Syntax_Expression( TToken **tkn, symtable_elem_t *gel )  {
  TList *StackOperator; // Stack Tokenu
  TList *ListPostFix; // List Tokenu
  StackOperator = ListInit();
  ListPostFix = ListInit();
  int genCode = 1; // generovani kodu povoleno

  if(StackOperator == NULL || ListPostFix == NULL) {
    // ERR_INTERNAL
    CallError(ERR_INTERNAL);   
    return 0;
  }

  symtable_elem_t *lel = NULL;
  TTokenType lastToken = TK_BRACKET_ROUND_LEFT;

  do {
    lel = NULL;

    if( CanBeTokenInExpression( (*tkn)->type ) && !CanBeTokenAfterToken( (*tkn)->type, lastToken ) ) {
      // neocekavany token vyrazu
      genCode = 0; // zakazano generovani kodu, ale neukoncuje se s chybou
      fprintf(stderr, ">>>genCode = 0\n");// DEBUG
      break; 
    }
    lastToken =  (*tkn)->type;

    if(  (*tkn)->type == TK_ID ) {
      if( ( lel = SymtableFind( gel->local_symtable,  (*tkn)->string ) ) == NULL ) {
        // ERR_SYN - Promena neexistuje
        CallError(ERR_SYN);
        fprintf(stderr, ">Expression: Promena/Parametr ID neexistuje.\n");
        break;
      }
      else {
        TListData data;
        data.pointer = (*tkn);
        if( !ListPushBack(ListPostFix, data) ) {
          // ERR_INTERNAL
          CallError(ERR_INTERNAL);
          break;
        }
      }
    }
    else if( (*tkn)->type == TK_NUM_INTEGER || (*tkn)->type == TK_NUM_DOUBLE || (*tkn)->type == TK_NUM_STRING ) {
      TListData data;
      data.pointer = (*tkn);
      if( !ListPushBack(ListPostFix, data) ) {
        // ERR_INTERNAL
        CallError(ERR_INTERNAL);
        break;
      }
    }
    else if(  (*tkn)->type == TK_BRACKET_ROUND_LEFT ) {
      TListData data;
      data.pointer = (*tkn);
      if( !ListPush(StackOperator, data) ) {
        // ERR_INTERNAL
        CallError(ERR_INTERNAL);
        break;
      }
    }
    else if(  (*tkn)->type == TK_PLUS ||  (*tkn)->type == TK_MINUS ||  (*tkn)->type == TK_MUL ||  (*tkn)->type == TK_DIV ||  (*tkn)->type == TK_DIV_INT ||  (*tkn)->type == TK_MOD ) {
      // Podle priority operatoru zpracuj operatory
      TListData data;
      while( ListFront(StackOperator, &data) && !CanBeOperatorPush( (*tkn)->type, ( (TToken*)data.pointer)->type ) ) {
        ListPushBack(ListPostFix, data);
        ListPop(StackOperator, &data);
      }//- while
      data.pointer = (*tkn);
      ListPush(StackOperator, data);
    }
    else if(  (*tkn)->type == TK_BRACKET_ROUND_RIGHT ) {
      TListData data;
      while(1) {
        if( !ListPop(StackOperator, &data) ) {
          // Pokud zasobnik op Neobsahuje '(' 
          // ERR_SYN
          CallError(ERR_SYN);
          fprintf(stderr, ">Expression: Vyraz obsahuje prebytecnou zavorku ')'\n");
          break;
        }
        else if( ((TToken*)data.pointer)->type == TK_BRACKET_ROUND_LEFT ) {
          break;
        }
        else {
          ListPushBack(ListPostFix, data); // dokud nenajde '(' tak operace sklada na vystupni retezec
        }

      }//-while
    }
    else {
      TListData data;
      while( ListPop(StackOperator, &data) ) {
        if( ((TToken*)data.pointer)->type == TK_BRACKET_ROUND_LEFT ) {
          // ERR_SYN
          CallError(ERR_SYN);
          fprintf(stderr, ">Expression: Neocekavana zavorka '('.\n");
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


  if( ListEmpty(ListPostFix) ) { // Pokud je vystupni Vyraz prazdny, tak neexistoval
    CallError(ERR_SYN);
    fprintf(stderr, ">Expression: Nebyl zadan vyraz.\n");
  }
  if( !ListEmpty(StackOperator) ) {
    CallError(ERR_SYN);
    fprintf(stderr, ">Expression: Nespravny vyraz zkontrolujte pocet operandu a operatoru.\n");
  }

  // ListPostFix obsahuje vyraz v posix podobe ktery je potreba vygenerovat
  if(!ERR_EXIT_STATUS && genCode) {
    // TODO 
    // GENERATOR
  }

  TListData data;

  // Uvolneni tokenu
  fprintf(stderr, ">>>ListPostFix\n");// DEBUG
  while( ListPop(ListPostFix, &data) ) {
    PrintToken( ((TToken*)data.pointer) );// DEBUG
    TokenDestroy( ((TToken*)data.pointer) );
  }
  fprintf(stderr, ">>>StackOperator\n");// DEBUG
  while( ListPop(StackOperator, &data) ) {
    PrintToken( ((TToken*)data.pointer) );// DEBUG
    TokenDestroy( ((TToken*)data.pointer) );
  }
  fprintf(stderr, ">>>\n");// DEBUG

  ListDestroy(StackOperator);
  ListDestroy(ListPostFix);

  return !ERR_EXIT_STATUS;
} //- int Syntax_Expression


int Syntax_ListExpression( TToken **tkn, symtable_elem_t *gel ) {

  if( (*tkn)->type == TK_EOL ) { // Pokud je to konec radku tak skonci
    (*tkn) = GetNextDestroyOldToken( (*tkn),1);
  }
  else if( Syntax_Expression(tkn, gel) ) { /// Expression

    if( (*tkn)->type == TK_SEMICOLON ) { /// Expresion ;

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);

      Syntax_ListExpression(tkn, gel); /// Expression ; ListExpresion
    }
    else {
      // ERR_SYN
      CallError(ERR_SYN);
      fprintf(stderr, ">ListExpression: Expression ;\n");
    }
  } 

  return !ERR_EXIT_STATUS;
} //- int Syntax_ListExpression

//
//------------------------------
//



int Syntax_ListParam(TToken **tkn, symtable_elem_t *gel, int isDeclareNow) { // isDeclareNow: 1=declare function; 0=function
  symtable_elem_t *lel = NULL;

  int existList = 0;
  if( (existList = ( gel->listParam != NULL ) ) ) {
    gel->listParam = ListInit();
  }

  int numParam = -1; // poradove cislo nacitaneho parametru - index
  while(1) {

    if( (*tkn)->type == TK_ID ) { /// ID
      lel = SymtableFind(GlobalSymtable, (*tkn)->string );
      if( lel != NULL ) { // parametr ma stejne jmeno jako existujici funkce
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">ListParam: Parametr ma stejne jmeno jako funkce\n");
        break;
      }
      numParam++;
      TListData data;

      if(existList) {
        if ( !ListRemove(gel->listParam, numParam, &data) ) { // Ziskej prvek pokud existuje
          // ERR_SYN - Nevhodny pocet parametru - rozna declarace a definice
          CallError(ERR_SYN);
          fprintf(stderr, ">ListParam: Nevhodny pocet parametru. Ruzne v declaraci a v definici.\n");
          break;
        }
      }
      else {
        // Prvek neexistuje, tak vytvor
        data.i = 0;
        data.pointer = NULL;
      }

      if(!isDeclareNow) { // Jedna se o definici funkce - ukladame nazev parametru
        lel = SymtableFind( gel->local_symtable , (*tkn)->string );
        if( lel != NULL ) { // parametr ma stejne jmeno jako existujici parametr
          // ERR_SYN
          CallError(ERR_SYN);
          fprintf(stderr, ">ListParam: Parametr ma stejne jmeno jako jiny parametr.\n");
          break;
        }
        lel = AddElemGlobal( gel->local_symtable ,  (*tkn)->string );
        lel->elemType = SYM_TYPE_PARAM;
        data.pointer = lel->name;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if( (*tkn)->type == TK_AS ) { /// ID as
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">ListParam: ID as\n");
        break;
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      int symDataType;
      if( (*tkn)->type == TK_INTEGER || (*tkn)->type == TK_DOUBLE || (*tkn)->type == TK_STRING ) { /// ID as DataType 
        switch( (*tkn)->type ) {
          case TK_INTEGER: symDataType = SYM_DATATYPE_INT; break; 
          case TK_DOUBLE: symDataType = SYM_DATATYPE_DOUBLE; break; 
          case TK_STRING: symDataType = SYM_DATATYPE_STRING; break; 
          default: break;
        }
      }
      else {
        // ERR_SYN
        CallError(ERR_SYN);
        fprintf(stderr, ">ListParam: ID as DataType\n");
        break;
      }

      if(existList) {
        // Pokud list i prvek existoval
        if( data.i != symDataType ) {
          // ERR_SYN - Parametr v declaraci a definici nejsou stejneho typu
          CallError(ERR_SYN);
          fprintf(stderr, ">ListParam: Parametry v declaraci a definici nejsou stejneho typu.\n");
          break;
        }
      }
      else {
        // Pokud list neexistoval tak jen uloz
        data.i = symDataType;
        if(!isDeclareNow) { // Jedna se o definici funkce - ukladame nazev parametru
          lel->dataType = symDataType;
        }
      }

      (*tkn) = GetNextDestroyOldToken( (*tkn),1);
      if( (*tkn)->type == TK_COMMA ) {
        (*tkn) = GetNextDestroyOldToken( (*tkn),1);
        continue;
      }

    }
    else {
      break;
    }


  }//- while(1)

  return !ERR_EXIT_STATUS;
} //- int Syntax_ListParam


int Syntax_ListInParam(TToken **tkn, symtable_elem_t *gel ) {

  return !ERR_EXIT_STATUS;
} //- int Syntax_ListInParam

#endif

// Soubor: src/parser.c


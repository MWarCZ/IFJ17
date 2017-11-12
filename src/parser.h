//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/parser.h                 //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "symtable.h"

TToken* GetNextDestroyOldToken(TToken *tkn, int canGetEOL);
int Syntax_Program();
int Syntax_ListParam(TToken *tkn, symtable_elem_t *gel, int isDeclareNow);
int Syntax_FunctBody(TToken *tkn, symtable_elem_t *gel );
int Syntax_ListDefVar(TToken *tkn, symtable_elem_t *gel );
int Syntax_ListCommand(TToken *tkn, symtable_elem_t *gel );
int Syntax_Condition(TToken *tkn, symtable_elem_t *gel );
int Syntax_RelationalOperator(TToken *tkn, symtable_elem_t *gel );
int Syntax_Expression(TToken *tkn, symtable_elem_t *gel );


#endif

// Soubor: src/parser.h


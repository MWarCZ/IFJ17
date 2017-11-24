//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/main.c                   //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#include <stdio.h>

#include "main.h"
#include "sample.h"
#include "parser2.h"
#include "error.h"

/**
 * @brief Hlavni funkce main.
 * @param argc Počet argumentu v argv.
 * @param argv Pole argumentů získaných z příkazové řádky.
 * @return Vraci 0 pokud aplikace proběhla vpořádku.
 */
int main() {

  SyntaxStartParse();

	return ERR_EXIT_STATUS;
}

// Soubor: src/main.c


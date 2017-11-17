//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/conversion.h             //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef CONVERSION_H
#define CONVERSION_H

#include "token.h"

/**
 * @brief Konvert retezce na int nebo double. V pripade TK_ID vola filtr na klicova slova.
 * @param token Ukazatel na strukturu TToken.
 */
void Convert(TToken *token);


/**
 * @brief Filtr na klicova slova.
 * @param token Ukazatel na strukturu TToken.
 */
void KeywordFilter(TToken *token);

#endif

// Soubor: src/conversion.h
//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: cesta/nazev_souboru.c        //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef ERROR_H
#define ERROR_H

typedef enum {
  TK_COMMA, // ,
  TK_EOF, // konec souboru
  TK_EOL, // znak konce radku
  TK_ID, // identifikator
  TK_SEMICOLON, // ;
  TK_TERM, // libovolny literal

  // klicova slova
  TK_AS,
  TK_ASC,
  TK_DECLARE,
  TK_DIM,
  TK_DO,
  TK_DOUBLE,
  TK_ELSE,
  TK_END,
  TK_CHR,
  TK_FUNCTION,
  TK_IF,
  TK_INPUT,
  TK_INTEGER,
  TK_LENGTH,
  TK_LOOP,
  TK_PRINT,
  TK_RETURN,
  TK_SCOPE,
  TK_STRING,
  TK_SUBSTR,
  TK_THEN,
  TK_WHILE,

  // rezervovana klicova slova
  TK_AND,
  TK_BOOLEAN,
  TK_CONTINUE,
  TK_ELSEIF,
  TK_EXIT,
  TK_FALSE,
  TK_FOR,
  TK_NEXT,
  TK_NOT,
  TK_OR,
  TK_SHARED,
  TK_STATIC,
  TK_TRUE,

  // ciselne konstanty
  TK_NUM_INTEGER,
  TK_NUM_DOUBLE,

  // porovnavaci operatory
  TK_EQUAL, // =
  TK_NOT_EQUAL, // <>
  TK_LESS, // <
  TK_GREATER, // >
  TK_LESS_EQUAL, // <=
  TK_GREATER_EQUAL, // >=

  // zavorky
  TK_BRACKET_ROUND_LEFT, // (
  TK_BRACKET_ROUND_RIGHT, // )

  // aritmeticke operatory
  TK_PLUS, // +
  TK_MINUS, // -
  TK_MUL, // *
  TK_DIV, // /
  TK_MOD, // \
}

typedef union {
  char* name; // identifikator
  int integerValue; // cela cisla
  double doubleValue; // desetinna cisla
  char charValue; // znak 'x'
  char* stringValue; // string "xxx"
} TTokenData;

typedef struct {
  TTokenType type; // typ tokenu
  TTokenData data; // data tokenu
  unsigned long int line; // cislo radku, kde se token nachazi
} TToken;

// Soubor: src/token.h
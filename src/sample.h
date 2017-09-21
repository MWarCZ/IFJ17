#ifndef SAMPLE_H
#define SAMPLE_H

#define HELLO_WORLD "Hello world!"

/**
 * @brief Funkce vypíše obsah kondtanty HELLO_WORLD  na stdout.
 */
void SayHelloWorld();

/**
 * @brief Funkce vraci číslo, které je funkci dáno jako parametr.
 * @param x Je to číslo, které bude vráceno.
 * @return Vrací číslo, které funkce obdržela.
 */
int GetNumber(int x);

#endif


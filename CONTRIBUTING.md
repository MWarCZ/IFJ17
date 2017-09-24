# Pokyny pro práci

---------------

## Práce se soubory

### Hlavička souborů
- Každý soubor bude začínat hlavičkou, která bude obsahovat **název projektu**, **název souboru**, **VUT přihlašovací jména** a **jména studentů**.

#### V souboru s jakzykem C
  ```c
//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: cesta/nazev_souboru.c        //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//////////////////////////////////////////
   ```
#### V souboru Makefile
  ```Makefile
##########################################
## Projekt: IFJ17                       ##
## Soubor: cesta/Makefile               ##
## Tým:                                 ##
##   xvalka05 Miroslav Válka            ##
##   xtrnen03 Jan Trněný                ##
##   xproko37 Lukáš Prokop              ##
##########################################
  ```

--------------

### Patička souborů
- Každý soubor bude obsahovat patičku s **názvem souboru**.

#### V souboru s jakzykem C
  ```
// Soubor: cesta/nazev_souboru.c
   ```
#### V souboru Makefile
  ```Makefile
## Soubor: cesta/Makefile
  ```

-------------

### Styl psaní kódu

#### Nastavení editoru
- Konce řádků
  - Linuxové řádky `LF`
- Znaková sada
  - `utf-8`
- Styl odsazování
  - Mezery
- Velikost odsazování
  - 2
- Vkládat prázdný řádek na konec souboru
- **Vyjímka je Makefile**
  - Styl odsazování
    - Tabulázory
> Pokud používáte editor podporující **.editorcontig**, tak dojde k automatickému nastavení editoru.

#### Estetická úprava kódu
- Názvy proměných
  - `camelCase`
- Názvy funkí
  - `CamelCase`
- Názvy konstant
  - `SNAKE_CASE`
- Styl závorek
  ```
  void HelloWorld() {
    ...c
    if (1 == 1) {
      ...
    }
    ...
  }
  ```
- Komentáře
  - Česky bez diakritiky
  - javadoc - [Doxygen](http://www.stack.nl/~dimitri/doxygen/manual/commands.html)
  ```c
    /**
   * Funkce vrati cislo, ktere dostane v parametru.
   * @param x Cislo, ktere bude vraceno.
   * @return Vrati cislo, ktere bylo v parametru x.
   */
   int GetNumber(int x) {
     return x;
   }
  ```

--------------

## Práce s repozitářem

### Pravidla
- Do větve `master` nikdo přímo **nebude** commitovat.
- Každý řešený problém, bude mít **vlastní větev**.
- Do jedné větve přispívá jen jeden člověk, aby se předcházelo kolizím.
  > Vyjímka je pomoc s vyřešením problémů bránící postupu práce dál. 
- `Pull request` musí potvrdit jiný člověk, než který o něj požádal.
  - Člověk, který bude přijímat `Pull request` musí zkontrolovat změny.
- Větve, které jsou již součástí větve `master` budou smazány.
  > Mohou nastat vyjímky u některých větví. _(např. experimental, refactoring, ...)_
- **`commit` se nerovná uložení!** `commit` se provádí po dokončení nějaké části řešeného problému.
  > Před commitem se ujistěte, že lze soubor zkompilovat.
  > Popřípadě, že prochází testy.

---------------

### Pár rad pro práci
- Před vytvořením nové větve z větve `master` aktualizujte repozitář.
  ```
  git pull
  ```
- Vytvoření nové větve a přepnutí se do ní.
  ```bash
  git checkout -b my_new_branch
  ```
- Odeslání větve do vzdáleného repositáře.
  ```
  git push origin my_new_branch
  ```
- Smazání lokální větve.
   ```
   git branch -d my_new_branch
   ```
   > Větev bude smazána jen pokud již je součástí větve `master`. <br />
   > Pokud ji budete chtít i přesto odstranit: <br />
   > `git branch -D my_new_branch`




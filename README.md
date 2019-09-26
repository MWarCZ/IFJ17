# IFJ17  [![Build Status](https://travis-ci.org/MWarCZ/IFJ17.svg?branch=master)][travis ci] [![Software License](https://img.shields.io/badge/license-MIT-blue.svg)][licence]

------------

## Cíl projektu
Cílem projektu je vytvořit překladač v jazyce C pro rádoby jazyk FreeBASIC, který bude překládán do jazyka IFJcode17.

> - Překladač bude překládat kód, který je zadán v zadáním a je inspirován jazyku FreeBASIC.
> - Jazyk IFJcode17 je mezikód, který bude předáván modulu interpretu. 
> 

------------

## Licence 
- **_Byli bychom rádi, kdyby studenti VUT FIT pracující na stejném/podobném projektu do předměntu IFJ, aby dbali na dodržení pravidel fakulty a předmětu k přebírání (respektivě nepřebírání) externího kódu. Děkujeme._**
- Repozitář je veřejný a kódy jsou distribuovány pod licencí [MIT][licence].

------------

## Kompilace projektu
- Používáme nástroj `make` pro usnadnění.
- Doporučujeme používat hlavní [Makefile][hlavni Makefile].
  - Makefile byl rozdělen na několik menších z důvodu přehlednosti.
  - Pomocné Makefily se nalézají v podsložkách projektu.
### Kompilace
- Projekt zkopmilujete pomocí nástroje `make`.
  ```
  [user@computer:~/IFJ17]$ make
  ```
  - Nebo
  ```
  [user@computer:~/IFJ17]$ make build
  ```
### Spouštění
- Pokud je projekt zkompilován, tak ve složce `build` je spustitelný soubor `run`.
  ```
  [user@computer:~/IFJ17]$ ./build/run
  ```


------------

## Testy a testování
### Co používáme
- [CHEAT](https://github.com/Tuplanolla/cheat)
  - Testovací framework pro psaní unit testů v jazyce C.
  - > Nestíhalo se => unit testy nejsou.
- ~~[Travis CI](https://travis-ci.org)~~
  - Automatizované spouštění testů pro kódy odeslané na GitHub.
  - Zdarma pro veřejné repozitáře.
  - > Nepoužito neboť repo musel být privátní.
### Jak spustit unit testy
- Testy je možné spustit za pomocí nástroje `make`
  ```
  [user@computer:~/IFJ17]$ make test
  ```
- Pokud nechcete testy spouštět, ale jen zkompilovat.
  ```
  [user@computer:~/IFJ17]$ make test-build
  ```
- Spouštět konkrétní soubor s testy je možné zatím jen takto:
  ```
  [user@computer:~/IFJ17]$ make test-build && ./build_test/sample.test.o
  ```

------------

## Členové týmu
- Miroslav Válka [![MWarCZ][github icon]](https://github.com/MWarCZ) [<img src='https://www.vutbr.cz/images/favicon_ostra.ico' width='24' />](https://www.vutbr.cz/lide/miroslav-valka-196460)
- Jan Trněný [![xtrnen][github icon]](https://github.com/xtrnen) [<img src='https://www.vutbr.cz/images/favicon_ostra.ico' width='24' />](https://www.vutbr.cz/lide/jan-trneny-196461)
- Lukáš Prokop [![Prokopl][github icon]](https://github.com/Prokopl) [<img src='https://www.vutbr.cz/images/favicon_ostra.ico' width='24' />](https://www.vutbr.cz/lide/lukas-prokop-196015)
- Pavel Bartoň [![pavel-barton][github icon]](https://github.com/pavel-barton) [<img src='https://www.vutbr.cz/images/favicon_ostra.ico' width='24' />](https://www.vutbr.cz/lide/pavel-barton-196255)

------------

## Pravidla pro praci
- Naleznete v souboru [CONTRIBUTING.md](CONTRIBUTING.md)
- **Pravidla nejsou na ozdobu!**

------------

[licence]: README.md
[travis ci]: https://travis-ci.org/MWarCZ/IFJ17
[hlavni Makefile]: Makefile
[github icon]: https://cdn1.iconfinder.com/data/icons/simple-icons/24/github-24-black.png



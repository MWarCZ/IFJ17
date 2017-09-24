##########################################
## Projekt: IFJ17                       ##
## Soubor: ./Makefile                   ##
## Tým:                                 ##
##   xvalka05 Miroslav Válka            ##
##   xtrnen03 Jan Trněný                ##
##   xproko37 Lukáš Prokop              ##
##########################################

.PHONY: all get-deps get-unittest build run test doc clean

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g

SOURCE_DIR=src/
BUILD_DIR=build/
DOCUMENTATION_DIR=doc/
TEST_DIR=test/
TEST_BUILD_DIR=/test_build/
TEST_FRAMEWORK_DIR=test_framework/

all: get-deps build
	@echo $@

# Stahne zavislosti potrebne ke kompilaci.
get-deps: get-unittest

# Stazeni frameworku pro unit testy.
get-unittest:
	cd $(TEST_DIR) && $(MAKE) get-deps

# Spusti zkompilovani unit testu.
test-build: build
	cd $(TEST_DIR) && $(MAKE)

# Zkompiluje testy a spusti je.
test: test-build
	cd $(TEST_DIR) && $(MAKE) run

# Zkompiluje projekt (bez testu).
build:
	@cd $(SOURCE_DIR) && $(MAKE)

# Spusti zkompilovany projekt.
run: build
	@cd $(SOURCE_DIR) && $(MAKE) run

%:
	@cd $(TEST_DIR) && $(MAKE) $@

# Zapne generovani automaticke dokumentace
doc:
	@if [ ! -d "$(DOCUMENTATION_DIR)" ]; then \
		mkdir "$(DOCUMENTATION_DIR)"; \
	fi
	doxygen

# Vymaze vsechny slozky a soubory, ktere neni potreba uchovavat v repozitari
clean:
	printf "\n===== CLEAN =====\n\n"
	rm -f -R "$(DOCUMENTATION_DIR)"
	@cd $(SOURCE_DIR) && $(MAKE) clean
	@cd $(TEST_DIR) && $(MAKE) clean


## Soubor: ./Makefile


##########################################
## Projekt: IFJ17                       ##
## Soubor: ./Makefile                   ##
## Tým:                                 ##
##   xvalka05 Miroslav Válka            ##
##   xtrnen03 Jan Trněný                ##
##   xproko37 Lukáš Prokop              ##
##   xbarto93 Pavel Bartoň              ##
##########################################

.PHONY: all get-deps get-unittest build run test doc clean

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g

# Slozka se zdrojovymi kody projektu
SOURCE_DIR=src/
# Slozka se zkompilovanymi soubory ze zdrojovych kodu
BUILD_DIR=build/
# Slozka s dokumentaci
DOXYGEN_DIR=doc/doxygen/
# Slozka se zdrojovymi kody unit testu
TEST_DIR=test/
# Slozka se zkompilovanymi unit testy
TEST_BUILD_DIR=build_test/
# Slozka s testovacim frameworkem pro unit testy
TEST_FRAMEWORK_DIR=test_framework/
# Jmeno vysledneho souboru = Jmeno IFJ17 prekladace
RUN_FILE_NAME=run
# aaa
ARCHIVE_DIR=archive/

all: build
	@echo $@

# Stahne zavislosti potrebne ke kompilaci.
get-deps: get-unittest

# Stazeni frameworku pro unit testy.
get-unittest:
	cd $(TEST_DIR) && $(MAKE) get-deps

# Spusti zkompilovani unit testu.
test-build: get-unittest build
	cd $(TEST_DIR) && $(MAKE) \
		BUILD_DIR=../$(TEST_BUILD_DIR) \
		TEST_FRAMEWORK_DIR=../$(TEST_FRAMEWORK_DIR) \
		SOURCE_OBJECTS_DIR=../$(BUILD_DIR)

# Zkompiluje testy a spusti je.
test: test-build
	cd $(TEST_DIR) && $(MAKE) run \
		BUILD_DIR=../$(TEST_BUILD_DIR) \
		TEST_FRAMEWORK_DIR=../$(TEST_FRAMEWORK_DIR) \
		SOURCE_OBJECTS_DIR=../$(BUILD_DIR)

# Zkompiluje projekt (bez testu).
build:
	@cd $(SOURCE_DIR) && $(MAKE) \
		BUILD_DIR=../$(BUILD_DIR) \
		RUN_FILE_NAME=$(RUN_FILE_NAME)

# Spusti zkompilovany projekt.
run: build
	@cd $(SOURCE_DIR) && $(MAKE) run \
		BUILD_DIR=../$(BUILD_DIR) \
		RUN_FILE_NAME=$(RUN_FILE_NAME)

# !!! Zatim nefunkcni
%:
	@cd $(TEST_DIR) && $(MAKE) $@

# Zapne generovani automaticke dokumentace
doc:
	mkdir -p "$(DOXYGEN_DIR)"; \
	doxygen

# !!! Zatim neni dokonceno
archive:
	printf "\n===== ARCHIVE =====\n\n"
	rm -Rf $(ARCHIVE_DIR)
	cp -R $(SOURCE_DIR) $(ARCHIVE_DIR)
	cp rozdeleni $(ARCHIVE_DIR)rozdeleni | true
	cd $(ARCHIVE_DIR) && zip xvalka05.zip *

# Vymaze vsechny slozky a soubory, ktere neni potreba uchovavat v repozitari
clean:
	printf "\n===== CLEAN =====\n\n"
	rm -f -R "$(DOXYGEN_DIR)"
	rm -f -R "$(ARCHIVE_DIR)"
	@cd $(SOURCE_DIR) && $(MAKE) clean \
		BUILD_DIR=../$(BUILD_DIR) \
		RUN_FILE_NAME=$(RUN_FILE_NAME)
	@cd $(TEST_DIR) && $(MAKE) clean \
		BUILD_DIR=../$(TEST_BUILD_DIR) \
		TEST_FRAMEWORK_DIR=../$(TEST_FRAMEWORK_DIR) \
		SOURCE_OBJECTS_DIR=../$(BUILD_DIR)


## Soubor: ./Makefile


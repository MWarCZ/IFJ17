
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

get-deps: get-unittest

get-unittest:
	cd $(TEST_DIR) && $(MAKE) get-deps

test-build: build
	cd $(TEST_DIR) && $(MAKE)

test: test-build
	cd $(TEST_DIR) && $(MAKE) run

build:
	@cd $(SOURCE_DIR) && $(MAKE)

run: build
	@cd $(SOURCE_DIR) && $(MAKE) run

%:
	@cd $(TEST_DIR) && $(MAKE) $@

doc:
	@if [ ! -d "$(DOCUMENTATION_DIR)" ]; then \
		mkdir "$(DOCUMENTATION_DIR)"; \
	fi
	doxygen

clean:
	printf "\n===== CLEAN =====\n\n"
	rm -f -R "$(DOCUMENTATION_DIR)"
	@cd $(SOURCE_DIR) && $(MAKE) clean
	@cd $(TEST_DIR) && $(MAKE) clean



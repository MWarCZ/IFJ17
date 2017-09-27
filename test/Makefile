##########################################
## Projekt: IFJ17                       ##
## Soubor: test/Makefile                ##
## Tým:                                 ##
##   xvalka05 Miroslav Válka            ##
##   xtrnen03 Jan Trněný                ##
##   xproko37 Lukáš Prokop              ##
##   xbarto93 Pavel Bartoň              ##
##########################################

.PHONY: all get-deps build-dir build run test clean

CC=gcc
CFLAGS=-std=c99 -g -I .

TEST_FRAMEWORK_DIR=../test_framework/
BUILD_DIR=../build_test/
SOURCE=$(wildcard *.test.c)
OBJECTS:=$(SOURCE:.c=.o)
OBJECTS:=$(addprefix $(BUILD_DIR),$(OBJECTS))

APP_OBJECTS:=$(filter-out ../build/run ../build/main.o,$(wildcard ../build/*))


all: build

get-deps:
	@if [ ! -d $(TEST_FRAMEWORK_DIR) ]; then \
		mkdir $(TEST_FRAMEWORK_DIR) \
		&& cd $(TEST_FRAMEWORK_DIR) \
		&& wget "http://github.com/Tuplanolla/cheat/raw/1.0.3/cheat.h"; \
		fi

build-dir:
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		mkdir $(BUILD_DIR); \
	fi

build: get-deps build-dir $(OBJECTS)

$(BUILD_DIR)run: $(OBJECTS)
	$(CC) $(CFLAGS) -c -o $@ $^ $(APP_OBJECTS)

### START: Dynamicke vytvoreni cilu s volitelnymi prerekvizitami

TARGETS=$(OBJECTS) 
define BUILD_TARGET 
$(1): $(wildcard $(1:$(BUILD_DIR)%.o=%.c)) $(wildcard $(1:$(BUILD_DIR)%.o=%.h)) 
	@#echo "$$@ >> $$^ <<" 
	$(CC) $(CFLAGS) -o $$@ $$^ $$(APP_OBJECTS)
endef 
$(foreach target,$(TARGETS),$(eval $(call BUILD_TARGET,$(target)))) 

### END 

run: build 
	@#printf "\n===== RUN =====\n\n"
	@#./$(BUILD_DIR)run
	@$(foreach run,$(OBJECTS), printf "\n===== TEST: $(subst $(BUILD_DIR),,$(run)) =====\n" && ./$(run);)

%: build $(BUILD_DIR)%.o 
	@printf "\n===== TEST =====\n" && ./$(word 2,$^)

clean:
	@printf "\n===== CLEAN =====\n\n"
	rm -f -R $(BUILD_DIR)
	rm -f -R $(TEST_FRAMEWORK_DIR)


## Soubor: test/Makefile


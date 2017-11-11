##########################################
## Projekt: IFJ17                       ##
## Soubor: src/Makefile                 ##
## Tým:                                 ##
##   xvalka05 Miroslav Válka            ##
##   xtrnen03 Jan Trněný                ##
##   xproko37 Lukáš Prokop              ##
##   xbarto93 Pavel Bartoň              ##
##########################################

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g -I .

BUILD_DIR=build/
SOURCE=$(wildcard *c)
OBJECTS:=$(SOURCE:.c=.o)
OBJECTS:=$(addprefix $(BUILD_DIR),$(OBJECTS))
RUN_FILE_NAME=run

all: build

build_dir:
	mkdir -p $(BUILD_DIR);

build: build_dir $(OBJECTS) $(BUILD_DIR)$(RUN_FILE_NAME)

$(BUILD_DIR)$(RUN_FILE_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

### START: Dynamicke vytvoreni cilu s volitelnymi prerekvizitami

TARGETS=$(OBJECTS) 
define BUILD_TARGET 
$(1): $(wildcard $(1:$(BUILD_DIR)%.o=%.c)) $(wildcard $(1:$(BUILD_DIR)%.o=%.h)) 
	@#echo "$$@ >> $$^ <<" 
	$(CC) $(CFLAGS) -c -o $$@ $$<
endef 
$(foreach target,$(TARGETS),$(eval $(call BUILD_TARGET,$(target)))) 

### END 

run: build
	@printf "\n===== RUN =====\n\n"
	@./$(BUILD_DIR)$(RUN_FILE_NAME)

clean:
	@printf "\n===== CLEAN =====\n\n"
	rm -f -R $(BUILD_DIR)


## Soubor: src/Makefile



CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g -I .

BUILD_DIR=../build/
SOURCE=$(wildcard *c)
OBJECTS:=$(SOURCE:.c=.o)
OBJECTS:=$(addprefix $(BUILD_DIR),$(OBJECTS))


all: build

build_dir:
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		mkdir $(BUILD_DIR); \
	fi

build: build_dir $(OBJECTS) $(BUILD_DIR)run

$(BUILD_DIR)run: $(OBJECTS)
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
	@./$(BUILD_DIR)run

clean:
	@printf "\n===== CLEAN =====\n\n"
	rm -f -R $(BUILD_DIR)


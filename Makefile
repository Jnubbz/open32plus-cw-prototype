# open32plus - host (PC) build: unit tests and simulator tools.
# The DM-32UV cross build will be added once the MCU is identified.

CC      ?= cc
CFLAGS  ?= -std=c11 -O2 -Wall -Wextra -Werror -pedantic
BUILD   := build

CORE_SRC := src/core/morse.c src/core/macro.c

.PHONY: all test sim clean

all: test sim

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/tests: tests/test_main.c $(CORE_SRC) | $(BUILD)
	$(CC) $(CFLAGS) -o $@ $^

test: $(BUILD)/tests
	./$(BUILD)/tests

$(BUILD)/cw2wav: platform/sim/cw2wav.c $(CORE_SRC) | $(BUILD)
	$(CC) $(CFLAGS) -D_DEFAULT_SOURCE -o $@ $^ -lm

sim: $(BUILD)/cw2wav

clean:
	rm -rf $(BUILD)

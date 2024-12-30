###############################################################################
# Generic Variables
###############################################################################
CMAKE := cmake
MAKE := make

BUILD_DIR := build

ROOT_MAKEFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOT_DIR := $(dir $(ROOT_MAKEFILE))
###############################################################################
# Variables
###############################################################################
.PHONY: build
Arguments := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

###############################################################################
# Build Commands
###############################################################################
all: test

init:
	@export IDF_TOOLS_PATH="$$HOME/miniconda3/envs/bristlemouth_esp32" && ./esp-idf/install.sh

build:
	@bash ./scripts/build.sh $(Arguments)

config:
	@bash ./scripts/config.sh $(Arguments)

flash:build
	@bash scripts/flash.sh $(Arguments)

monitor:
	@bash scripts/monitor.sh $(Arguments)

clean:
	@rm -rf build

%::
	@true

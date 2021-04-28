SKETCH				:= src/main.cpp
LIBS				:= src/utils.cpp

BUILD_DIR			:= build

CHIP				:= esp8266
BOARD				:= nodemcuv2

# BUILD_EXTRA_FLAGS	:= -Iinclude

VERBOSE				:= 1

# Include the `makeEspArduino` Makefile
include tools/makefile/makeEspArduino.mk
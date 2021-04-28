SKETCH				:= src/main.cpp
LIBS				:= include/ $(wildcard src/*.cpp)

BUILD_DIR			:= build/

CHIP				:= esp8266
BOARD				:= nodemcuv2

COMP_WARNINGS		:= -Wall -Werror
VERBOSE				:= 1

# Include the `makeEspArduino` Makefile
include tools/makefile/makeEspArduino.mk
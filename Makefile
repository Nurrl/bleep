SKETCH				:= src/main.cpp
LIBS				:= include/ $(wildcard src/*.cpp)
ENV					:= .env

BUILD_DIR			:= build/

CHIP				:= esp8266
BOARD				:= nodemcuv2
FLASH_DEF			:= 4M1M

COMP_WARNINGS		:= -Wall -Werror
VERBOSE				:= 1

#- `.env` file loading for secret management -#
ifneq ($(strip $(ENV)),)

# Include the environment file to get acces to variables
include $(ENV)

# Identify the variables to add them to `BUILD_EXTRA_FLAGS`
DEFINES	:= $(shell awk '{print $$1;}' $(ENV))
DEFINES := $(foreach define,$(DEFINES),-D$(define)='$($(define))')

BUILD_EXTRA_FLAGS += $(DEFINES)

endif

# Include the `makeEspArduino` Makefile
include tools/makefile/makeEspArduino.mk

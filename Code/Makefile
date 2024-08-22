

EXECUTABLE_NAME = main
FILE_EXTENSION = cpp

INCLUDE_DIR = include
SOURCE_DIR = src
BUILD_DIR = build

DEBUG ?= 0


CXX = g++
CXX_STANDARD = c++23
# -Werror
CXX_WARNINGS = -Wall -Wextra -pedantic -Wunused -Wuninitialized -Wconversion
CXXFLAGS = $(CXX_WARNINGS) -std=$(CXX_STANDARD) -I $(INCLUDE_DIR) -frounding-math


ifeq ($(DEBUG), 1)
CXXFLAGS += -g -O0
else
CXXFLAGS += -O3
endif


COMPILER_CALL = $(CXX) $(CXXFLAGS)

CXX_SOURCES = $(wildcard $(SOURCE_DIR)/*.$(FILE_EXTENSION))
CXX_OBJECTS = $(patsubst $(SOURCE_DIR)/%.$(FILE_EXTENSION), $(BUILD_DIR)/%.o, $(CXX_SOURCES))


all: create build


create:
	mkdir -p build
	mkdir -p input


build: create $(CXX_OBJECTS)
	$(COMPILER_CALL) $(CXX_OBJECTS) -o $(BUILD_DIR)/$(EXECUTABLE_NAME) -lgmp



execute: build
	./$(BUILD_DIR)/$(EXECUTABLE_NAME)


clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f $(BUILD_DIR)/$(EXECUTABLE_NAME)


clean_output:
	rm -r output



# $@: the file name of the target
# $<: the name of the first dependency
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.$(FILE_EXTENSION)
	$(COMPILER_CALL) -c $< -o $@


.PHONY: all create execute clean


INC_DIR := ./inc
SRC_DIR := ./src
EXE_DIR := ./bin
TARGET := $(EXE_DIR)/main.exe
CPPFLAGS := -g -o 
CC := g++
OBJS_CPP :=$(wildcard  $(SRC_DIR)/*.cpp)
OBJS_HPP :=$(wildcard  $(INC_DIR)/*.h)
$(TARGET):$(OBJS_CPP) $(OBJS_HPP)
	$(CC) $(CPPFLAGS) $@ $(OBJS_CPP)  -m32 -DOS_LINUX -lICTCLAS50 -ljson -I$(INC_DIR) -Wno-deprecated

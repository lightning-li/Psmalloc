# Makefile for PSMalloc

# Get system machine hardware
MACH_HARDWARE = $(shell uname -m)
ifeq ($(MACH_HARDWARE), x86_64)
SYS_LIB_DIR = /usr/lib64
else
SYS_LIB_DIR = /usr/lib
endif

DIR_SRC = ./src
SRC = $(wildcard ${DIR_SRC}/*.c)
OBJ = $(patsubst %.c, %.o, $(notdir ${SRC}))
TEST = ./test/test.c

CC = gcc
TARGET = libpsmalloc.so
CFLAGS = -g -Wall -fpic -lpthread 

$(TARGET): $(OBJ)
	$(CC) -fpic -shared -o $@ $^

$(OBJ) : $(SRC)
	$(CC) $(CFLAGS) -c $^

.PHONY:install
install:
	mv $(TARGET) $(SYS_LIB_DIR)

.PHONY:test
test:
	$(CC) -lpthread $(TEST) -o ptmalloc.test
	$(CC) -lpthread -lpsmalloc $(TEST) -o psmalloc.test
	@echo "ptmalloc:"
	@./ptmalloc.test
	@echo "psmalloc:"
	@./psmalloc.test

.PHONY:distclean
distclean:
	rm -f *.o

.PHONY:clean
clean:
	rm -f *.o *.so *.test

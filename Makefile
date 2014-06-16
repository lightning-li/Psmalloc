
# Copyright (C) FillZpp


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
CFLAGS = -g -Wall -O2

$(TARGET): $(OBJ)
	$(CC) -fpic -shared -lpthread -o $@ $^

$(OBJ) : $(SRC)
	$(CC) $(CFLAGS) -fpic -c $^

.PHONY:install
install:
	mv $(TARGET) $(SYS_LIB_DIR)

.PHONY:test
test:
	$(CC) -lpthread $(TEST) -o test_libc
	$(CC) -lpthread -ltcmalloc $(TEST) -o test_tcmalloc
	$(CC) -lpthread -lpsmalloc $(TEST) -o test_psmalloc
	@echo -e "\nlibc:"
	@./test_libc 100
	@echo -e "\ntcmalloc:"
	@./test_tcmalloc 100
	@echo -e "\npsmalloc:"
	@./test_psmalloc 100
	@rm -f test_libc test_tcmalloc test_psmalloc

.PHONY:distclean
distclean:
	rm -f *.o

.PHONY:clean
clean:
	rm -f *.o *.so *.test

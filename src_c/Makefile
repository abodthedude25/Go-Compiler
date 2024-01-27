# Makefile for C-Flex Example
# For 411 Tutorials W2023
# Shankar Ganesh

# The substitutions and wildcards can be done in one or two lines
# I have broken it down so you can understand what is going on

# Added on: Abdelrahman Abbas
CC := gcc
CCFLAGS := -Wall -O2
LEX := flex
PARSE := bison
SRC = $(wildcard *.c)
LEXIN := $(wildcard *.l)
LEXOUT := $(LEXIN:.l=.yy.c)

PARSEIN := $(wildcard *.y)
PARSEOUT := $(PARSEIN:.y=.tab.c)

EXEC := golf
OBJS = $(SRC:.c=.o)
OBJS += $(LEXOUT:.c=.o)
OBJS += $(PARSEOUT:.c=.o)

all: $(PARSEOUT) $(LEXOUT) $(OBJS) 
	@echo -n "Generating executable... "
	@$(CC) $(CCFLAGS) -o $(EXEC) $(OBJS)
	@echo "Done"
	@rm $(LEXOUT)
	@rm $(PARSEOUT)

%.tab.c: %.y
	@echo -n "Generating parser using $<... "
	@bison $<
	@echo "Done"

%.yy.c: %.l
	@echo -n "Generating scanner using $<... "
	@flex -o $@ $<
	@echo "Done"

-include $(OBJS:.o=.d)

%.o: %.c
	@echo -n "Generating object $@... "
	@$(CC) $(CCFLAGS) -MMD -MF $*.d -c $<
	@echo "Done"

# .PHONY=clean
clean:
	-rm -rf $(OBJS) $(EXEC) $(LEXOUT) $(PARSEOUT) *.d
	-rm *.tab.c
	-rm *.yy.c
	-rm *.tab.h

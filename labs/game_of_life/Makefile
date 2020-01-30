.PHONY: print compile default
.DEFAULT: compile

#SRC FILES
TARGET_EXEC ?= a.out
SRCD?=src
OBJD?=obj
SRCS=$(notdir $(wildcard $(SRCD)/*.c))
OBJS=$(addprefix $(OBJD)/, $(SRCS:.c=.o))
DIRS=$(OBJD)

STD_CC?=gcc
INCLUDE?= -I./inc

default: compile

obj:
	mkdir -p $@
	touch $@/.gitignore

$(OBJS): $(OBJD)/%.o : src/%.c
	$(STD_CC) $(INCLUDE) -o $@ $<

print:
	@echo $(SRCS)
	@echo $(OBJS)

compile: $(DIRS) $(OBJS)

clean:
	rm -rf $(OBJD)

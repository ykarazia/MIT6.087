.PHONY: print compile default link
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

default: link

obj:
	mkdir -p $@
	touch $@/.gitignore

$(OBJS): $(OBJD)/%.o : src/%.c
	$(STD_CC) -c -g $(INCLUDE) -o $@ $<

$(TARGET_EXEC) : $(OBJS)
	$(STD_CC) -g -o $@ $^ 

print:
	@echo $(SRCS)
	@echo $(OBJS)

compile: $(DIRS) $(OBJS)

link: $(DIRS) $(TARGET_EXEC)

clean:
	rm -rf $(OBJD)
	rm $(TARGET_EXEC)

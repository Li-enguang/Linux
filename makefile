
.PHONY:	main

SRCS=$(wildcard $(SRCDIR)/*.cpp)
DIR=$(notdir $(SRCS))
#OBJS=$(patsubst %.cpp,%.o,$(DIR))
OBJS=$(DIR:.cpp=.o)

BASEDIR=$(shell pwd)
OBJDIR=$(BASEDIR)/obj
SRCDIR=$(BASEDIR)/src

OBJS:=$(addprefix $(OBJDIR)/, $(OBJS))
$(shell mkdir -p $(OBJDIR))

C=g++

main : $(OBJS)
	$(C) $^ -o $@
$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(C) -c $< -o $@
clean :
	rm -rf $(OBJS)

TARGET := instruction.a

SRCS := parse.cpp exec.cpp emu.cpp instr_base.cpp instr16.cpp instr32.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

CXXFLAGS := -Wall -MMD -std=c++11 -I../include $(DEBUG)

.PHONY: all
all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJS)
	$(AR) rcs $@ $^

.PHONY: clean
clean:
	$(RM) $(DEPS) $(OBJS) $(TARGET)


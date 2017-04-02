TARGET := instruction.a

SRCS := parse.cpp exec.cpp instr_base.cpp instr16.cpp instr32.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

CXXFLAGS := -Wall -MMD -std=c++11 -I../include

all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJS)
	$(AR) rcs $@ $^

clean:
	$(RM) $(DEPS) $(OBJS) $(TARGET)


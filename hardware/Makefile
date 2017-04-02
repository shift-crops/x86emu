TARGET := hardware.a

SRCS := processor.cpp memory.cpp eflags.cpp io.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

CXXFLAGS := -Wall -MMD -std=c++11 -I../include

all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJS)
	$(AR) rcs $@ $^

clean:
	$(RM) $(DEPS) $(OBJS) $(TARGET)

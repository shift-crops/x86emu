TARGET := emulator.a

SRCS := emulator.cpp access.cpp interrupt.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

CXXFLAGS := -Wall -MMD -I../include

all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJS)
	$(AR) rcs $@ $^

clean:
	$(RM) $(DEPS) $(OBJS) $(TARGET)

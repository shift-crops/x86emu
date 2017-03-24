TARGET := x86emu

SRCS := main.cpp debug.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

SUB_OBJS := hardware/hardware.a
SUB_OBJS += instruction/instruction.a

CXXFLAGS := -Wall -MMD

$(TARGET): $(OBJS) $(SUB_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

-include $(DEPS)

$(SUB_OBJS):
	make -C hardware
	make -C instruction

all:
	make -C hardware
	make -C instruction
	make $(TARGET)

clean:
	make clean -C hardware
	make clean -C instruction
	$(RM) $(DEPS) $(OBJS) $(TARGET)

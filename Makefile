TARGET := x86emu

SRCS := main.cpp debug.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

SUB_OBJS := hardware/hardware.a
SUB_OBJS += emulator/emulator.a
SUB_OBJS += instruction/instruction.a
SUB_OBJS += device/device.a

CXXFLAGS := -Wall -MMD -std=c++11 -I./include
LDFLAGS  := -lglfw -lGL -lpthread

$(TARGET): $(OBJS) $(SUB_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

-include $(DEPS)

$(SUB_OBJS):
	make -C hardware
	make -C emulator
	make -C instruction
	make -C device

_emu:
	make -C hardware
	make -C emulator
	make -C instruction
	make -C device
	make $(TARGET)

_soft:
	make -C bios
	make -C sample

all:
	make _emu
	make _soft

clean:
	make clean -C hardware
	make clean -C emulator
	make clean -C instruction
	make clean -C device
	make clean -C bios
	make clean -C sample
	$(RM) $(DEPS) $(OBJS) $(TARGET)

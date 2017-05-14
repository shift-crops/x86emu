TARGET := x86emu

SRCS := main.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

SUB_OBJS := emulator/emulator.a
SUB_OBJS += instruction/instruction.a
SUB_OBJS += hardware/hardware.a
SUB_OBJS += device/device.a
SUB_OBJS += util/util.a

CXXFLAGS := -Wall -MMD -std=c++11 -I./include $(DEBUG)

LDFLAGS  := -lpthread

UNAME = ${shell uname}
ifeq ($(OS),Windows_NT)
	LDFLAGS  += -lglfw3 -lopengl32
else ifeq ($(UNAME),Linux)
	LDFLAGS  += -lglfw -lGL
else ifeq ($(UNAME),Darwin)
	LDFLAGS  += -lglfw -framework OpenGL
endif

$(TARGET): $(OBJS) $(SUB_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

-include $(DEPS)

$(SUB_OBJS): emu

.PHONY: emu
emu:
ifdef DEBUG
	make -C hardware DEBUG=$(DEBUG)
	make -C emulator DEBUG=$(DEBUG)
	make -C instruction DEBUG=$(DEBUG)
	make -C device DEBUG=$(DEBUG)
	make -C util DEBUG=$(DEBUG)
else
	make -C hardware
	make -C emulator
	make -C instruction
	make -C device
	make -C util
endif

.PHONY: os
os:
	make -C bios
	make -C sample

.PHONY: all
all: emu $(TARGET) os

.PHONY: clean
clean:
	make clean -C hardware
	make clean -C emulator
	make clean -C instruction
	make clean -C device
	make clean -C util
	make clean -C bios
	make clean -C sample
	$(RM) $(DEPS) $(OBJS) $(TARGET)

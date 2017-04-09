TARGET := device.a

SRCS := com.cpp syscontrol.cpp vram.cpp pic.cpp pit.cpp fdd.cpp vga.cpp keyboard.cpp mouse.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

CXXFLAGS := -Wall -MMD -std=c++11 -I../include

all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJS)
	$(AR) rcs $@ $^

clean:
	$(RM) $(DEPS) $(OBJS) $(TARGET)

TARGET := device.a

#SRCS := com.cpp syscontrol.cpp vram.cpp display.cpp
SRCS := com.cpp syscontrol.cpp vram.cpp picreg.cpp pit.cpp fdd.cpp
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

CXXFLAGS := -Wall -MMD -std=c++11 -I../include

all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJS)
	$(AR) rcs $@ $^

clean:
	$(RM) $(DEPS) $(OBJS) $(TARGET)

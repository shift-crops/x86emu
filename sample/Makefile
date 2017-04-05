TARGET := kernel.img

TGT1 := boot.bin
TGT2 := kernel.bin
OBJS := crt0.o main.o

CC	:= gcc
AS	:= nasm
CFLAGS  := -nostdlib -fno-asynchronous-unwind-tables -g -fno-stack-protector -m32
LDFLAGS := --entry=start --oformat=binary -Ttext 0x0 -melf_i386

all :
	make $(TARGET)

%.o : %.c Makefile
	$(CC) $(CFLAGS) -c $<

%.o : %.asm Makefile
	$(AS) -f elf $<

$(TARGET) : $(TGT1) $(TGT2)
	cat $^ > $@

$(TGT1) : boot.asm
	$(AS) $< -o $@

$(TGT2) : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

clean :
	$(RM) $(OBJS) $(TGT1) $(TGT2) $(TARGET)

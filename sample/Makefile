TARGET := kernel.img

TGT1 := boot.bin
TGT2 := kernel.bin
OBJS := startup.o funcs.o init.o main.o keyboard.o vga.o

CC	:= gcc
AS	:= nasm
CFLAGS  += -masm=intel -nostdlib -fno-asynchronous-unwind-tables -fcf-protection=none -fno-stack-protector -fno-pie -m32
LDFLAGS := --entry=start --oformat=binary -Ttext 0x10000 -melf_i386

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

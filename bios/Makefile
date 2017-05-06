TARGET	= bios.bin
OBJS	= startup.o bios.o functions.o service.o disk.o vga.o video.o keyboard.o

CC 	:= gcc
AS 	:= nasm
CFLAGS	+= -masm=intel -nostdlib -fno-asynchronous-unwind-tables -g -fno-stack-protector -m16
LDFLAGS	+= --entry=start --oformat=binary -Ttext 0x0 -melf_i386

all :
	make $(TARGET)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

%.o : %.asm
	$(AS) -f elf $<

crt0.bin : crt0.asm
	$(AS) $< -o $@

$(TARGET) : $(OBJS) crt0.bin
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET) crt0.bin

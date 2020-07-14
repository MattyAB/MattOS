C_SOURCES = $(wildcard src/kernel/*.c src/driver/*.c)
HEADERS = $(wildcard src/kernel/*.h src/drivers/*.h)

OBJ = ${C_SOURCES:.c=.o}


all: os-image

os-image : build/boot/boot_sect.bin build/kernel/kernel.bin
	cat $^ > $@


build/boot/long_mode.bin : src/boot/enter_long_mode.asm
	nasm $< -f bin -o $@ -i src/boot/
	
build/boot/boot_sect.bin : src/boot/boot_sect.asm
	nasm $< -f bin -o $@ -i src/boot/


build/kernel/kernel.o : src/kernel/kernel.c
	gcc -ffreestanding -c $< -o $@

build/kernel/kernel_entry.o : src/kernel/kernel_entry.asm
	nasm $< -f elf64 -o $@ -i src/boot/

build/kernel/kernel.bin : build/kernel/kernel_entry.o ${OBJ}
	ld -o $@ -Ttext 0x9000 $^ --oformat binary


%.o : %.c ${HEADERS}
	gcc -ffreestanding -c $< -o $@
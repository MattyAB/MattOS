C_SOURCES = $(wildcard src/kernel/*.c src/driver/*.c)
HEADERS = $(wildcard src/kernel/*.h src/driver/*.h)

OBJ = ${C_SOURCES:.c=.o}

run: os-image
	qemu-system-x86_64 os-image

all: os-image

os-image : build/boot/boot_sect.bin build/kernel/kernel.bin
	cat $^ > $@


build/boot/long_mode.bin : src/boot/enter_long_mode.asm
	nasm $< -f bin -o $@ -i src/boot/
	
build/boot/boot_sect.bin : src/boot/boot_sect.asm
	nasm $< -f bin -o $@ -i src/boot/


build/kernel/interrupthandle.o : src/kernel/interrupthandle.asm
	nasm $< -f elf -o $@

build/kernel/kernel_entry.o : src/kernel/kernel_entry.asm
	nasm $< -f elf -o $@ -i src/boot/

build/kernel/kernel.bin : build/kernel/kernel_entry.o build/kernel/interrupthandle.o ${OBJ}
	ld -o $@ -Ttext 0x9000 $^ --oformat binary -m elf_i386


%.o : %.c ${HEADERS}
	gcc -ffreestanding -fno-pie -c $< -o $@ -m32 -g

clean:
	rm ${OBJ}
	rm build/boot/boot_sect.bin build/kernel/kernel.bin

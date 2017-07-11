arch ?= x86_64
kernel := build/kernel-$(arch).bin
iso := build/os-$(arch).iso
target ?= src/cpp_main.cpp
cpp := bin/robos.o

linker_script := src/arch/$(arch)/linker.ld
grub_cfg := src/arch/$(arch)/grub.cfg
assembly_source_files := $(wildcard src/arch/$(arch)/*.asm)
assembly_object_files := $(patsubst src/arch/$(arch)/%.asm,	build/arch/$(arch)/%.o, $(assembly_source_files))

.PHONY: all clean run iso

all: $(kernel)

clean:
	@rm -rf build
	@rm -rf bin

run: $(iso)
	@/usr/local/Cellar/qemu/2.9.0/bin/qemu-system-x86_64 -cdrom $(iso)

debug: $(iso)
	@/usr/local/Cellar/qemu/2.9.0/bin/qemu-system-x86_64 -cdrom $(iso) -s -S

# gdb:
# 	@/usr/local/bin/x86_64-pc-elf-g++ -std=c++1z -Wall -Werror -nostdlib -nostartfiles -ffreestanding -mno-red-zone -c $(target) -o $(cpp)	

iso: $(iso)

$(iso): $(kernel) $(grub_cfg)
	@mkdir -p build/isofiles/boot/grub
	@cp $(kernel) build/isofiles/boot/kernel.bin
	@cp $(grub_cfg) build/isofiles/boot/grub
	@/Users/robertbaldwin/opt/bin/grub-mkrescue -o $(iso) build/isofiles 2> /dev/null
	@rm -r build/isofiles

$(kernel): kernel $(cpp) $(assembly_object_files) $(linker_script)
	@/usr/local/bin/x86_64-pc-elf-ld -n --gc-sections --build-id=none -T $(linker_script) -o $(kernel) $(assembly_object_files) $(cpp)

kernel: 
	@mkdir -p bin/
	@/usr/local/bin/x86_64-pc-elf-g++ -std=c++1z -Wall -fpermissive -nostdlib -nostartfiles -ffreestanding -mno-red-zone -c $(target) -o $(cpp)

# compile assembly files
build/arch/$(arch)/%.o: src/arch/$(arch)/%.asm
	@mkdir -p $(shell dirname $@)
	@/usr/local/bin/nasm -felf64 $< -o $@

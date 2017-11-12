#include "io.hpp"
#include "multiboot.hpp"
#include "memory.hpp"

extern "C" {

 extern unsigned int magic;
 extern unsigned int mbi;

int cpp_main(int argc, const char** argv) {
    tiny_os::io::console::clear();
    tiny_os::io::console::printf("5/2 is %d\n", 5/2);
    tiny_os::io::console::printf("Hi, and welcome to TinyOS, date: %d-%s-%s\n", 2017, "07", "04");
    tiny_os::io::console::printf("magic number 0x%x\n", magic);

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        tiny_os::io::console::printf("invalid magic number %x\n", MULTIBOOT_BOOTLOADER_MAGIC);
        return 0;
    }
    if (mbi & 7) {
        tiny_os::io::console::printf("unaligned mbi 0x%x\n", mbi);
    }
    tiny_os::io::console::printf("multiboot address 0x%x\n", mbi);

    tiny_os::multiboot::print_info(mbi);

    tiny_os::io::console::printf("size of memory header %d\n", tiny_os::memory::memory_node_size);

    tiny_os::memory::allocator<char> byte_allocator(0x0100000, 0x7ee0000 / 4); // freaks out if range is too large? need to investigate
    char* string = byte_allocator.allocate(20);
    tiny_os::io::console::printf("string location 0x%x\n", (tiny_os::size_type)string);
    char buffer[13] = "Hi there %s!";
    tiny_os::memcpy(buffer, 13, string, 20);

    tiny_os::io::console::printf(string, "Rob");
    char* result = byte_allocator.allocate(1);
    __BLOCK
    *result = tiny_os::io::console::getchar();
    //printf("\n\ngot char c = \'%c\' from 0x%x", *result, *result);
    __BLOCK

    byte_allocator.deallocate(result);
    byte_allocator.deallocate(string);

    __BLOCK // For debugging
    return 0;
}

}

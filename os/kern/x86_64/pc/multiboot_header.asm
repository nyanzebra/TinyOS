section .multiboot_header ; https://en.wikipedia.org/wiki/Executable_and_Linkable_Format#File_header
header_start:

    ; boot http://nongnu.askapache.com/grub/phcoder/multiboot.pdf sec 3.12
    dd 0xE85250D6 ; load in magic #
    dd 0 ; load protected_mode_arch
    dd header_end - header_start ; specify length of header
    ; generate checksum
    dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start))

    ; optional flags
    ; see sec 3.12 https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
    dw 0 ; tag
    dw 0 ; flags
    dd 8 ; size of tag

header_end:

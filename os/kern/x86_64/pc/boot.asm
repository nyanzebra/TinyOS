global magic
global mbi

; https://github.com/RWTH-OS/eduOS/blob/42293ad056fb9bb8ab7f14473821849ca416898a/arch/x86/kernel/entry.asm
section .multiboot_header ; https://en.wikipedia.org/wiki/Executable_and_Linkable_Format#File_header
global start
extern long_mode_start
ALIGN 4
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
gdt64:
	.null: equ $ - gdt64         ; The null descriptor.
	dw 0                         ; Limit (low).
	dw 0                         ; Base (low).
	db 0                         ; Base (middle)
	db 0                         ; Access.
	db 0                         ; Granularity.
	db 0                         ; Base (high).
	.code: equ $ - gdt64         ; The code descriptor.
	dw 0                         ; Limit (low).
	dw 0                         ; Base (low).
	db 0                         ; Base (middle)
	db 10011000b                 ; Access.
	db 00100000b                 ; Granularity.
	db 0                         ; Base (high).
	.data: equ $ - gdt64         ; The data descriptor.
	dw 0                         ; Limit (low).
	dw 0                         ; Base (low).
	db 0                         ; Base (middle)
	db 10010010b                 ; Access.
	db 00000000b                 ; Granularity.
	db 0                         ; Base (high).
	.pointer:
  dw $ - gdt64 - 1
  dq gdt64

section .text
bits 32
start:
    mov esp, stack_top
    mov [magic], eax
    mov [mbi], ebx
    ;mov edi, ebx ; pass multiboot info, will be available by cpp_main

    call check_multiboot
    call check_cpuid
    call check_long_mode
    call set_up_page_tables
    call enable_paging
    call set_up_SSE

    ; load the 64-bit GDT
    lgdt [gdt64.pointer]

    jmp gdt64.code:long_mode_start

error:
    ; print ERR
    mov dword [0xb8000], 0x4f524f45
    mov dword [0xb8004], 0x4f3a4f52
    mov dword [0xb8008], 0x4f204f20
    mov byte  [0xb800a], al
    hlt

check_long_mode:
    ; test if extended processor info in available
    mov eax, 0x80000000    ; implicit argument for cpuid
    cpuid                  ; get highest supported argument
    cmp eax, 0x80000001    ; it needs to be at least 0x80000001
    jb .no_long_mode       ; if it is less, the CPU is too old for long mode

    ; use extended info to test if long mode is available
    mov eax, 0x80000001    ; argument for extended processor info
    cpuid                  ; returns various feature bits in ecx and edx
    test edx, 1 << 29      ; test if the LM-bit is set in the D-register
    jz .no_long_mode       ; If it is not set, there is no long mode
    ret
.no_long_mode:
    mov al, "2"
    jmp error

check_cpuid:
    ; Check if CPUID is supported by attempting to flip the ID bit (bit 21)
    ; in the FLAGS register. If we can flip it, CPUID is available.

    ; Copy FLAGS in to EAX via stack
    pushfd
    pop eax

    ; Copy to ECX as well for comparing later on
    mov ecx, eax

    ; Flip the ID bit
    xor eax, 1 << 21

    ; Copy EAX to FLAGS via the stack
    push eax
    popfd

    ; Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
    pushfd
    pop eax

    ; Restore FLAGS from the old version stored in ECX (i.e. flipping the
    ; ID bit back if it was ever flipped).
    push ecx
    popfd

    ; Compare EAX and ECX. If they are equal then that means the bit
    ; was not flipped, and CPUID is not supported.
    cmp eax, ecx
    je .no_cpuid
    ret
.no_cpuid:
    mov al, "1"
    jmp error

check_multiboot:
    cmp eax, 0x36d76289
    jne .no_multiboot
    ret
.no_multiboot:
    mov al, "0"
    jmp error

set_up_page_tables:
    ; recursive mapping
    mov eax, p4_table
    or eax, 0b11 ; present + writable
    mov [p4_table + 511 * 8], eax

    ; map first P4 entry to P3 table
    mov eax, p3_table
    or eax, 0b11 ; present + writable
    mov [p4_table], eax

    ; map first P3 entry to P2 table
    mov eax, p2_table
    or eax, 0b11 ; present + writable
    mov [p3_table], eax

    ; map each P2 entry to a huge 2MiB page
    mov ecx, 0         ; counter variable
.map_p2_table:
    ; map ecx-th P2 entry to a huge page that starts at address 2MiB*ecx
    mov eax, 0x200000  ; 2MiB
    mul ecx            ; start address of ecx-th page
    or eax, 0b10000011 ; present + writable + huge
    mov [p2_table + ecx * 8], eax ; map ecx-th entry

    inc ecx            ; increase counter
    cmp ecx, 512       ; if counter == 512, the whole P2 table is mapped
    jne .map_p2_table  ; else map the next entry

    ret

; Check for SSE and enable it. If it is not supported throw error.
set_up_SSE:
    ; check for SSE
    mov eax, 0x1
    cpuid
    test edx, 1<<25
    jz .no_SSE

    ; enable SSE
    mov eax, cr0
    and ax, 0xFFFB      ; clear coprocessor emulation CR0.EM
    or ax, 0x2          ; set coprocessor monitoring  CR0.MP
    mov cr0, eax
    mov eax, cr4
    or ax, 3 << 9       ; set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
    mov cr4, eax

    ret
.no_SSE:
    mov al, "a"
    jmp error

enable_paging:
    ; load P4 to cr3 register (cpu uses this to access the P4 table)
    mov eax, p4_table
    mov cr3, eax

    ; enable PAE-flag in cr4 (Physical Address Extension)
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; set the long mode bit in the EFER MSR (model specific register)
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; enable paging in the cr0 register
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret

section .rodata

section .data
magic:
    dd 1
mbi:
    dd 1

section .bss
; paging
align 4096
p4_table: ;the Page-Map Level-4 Table (PML4),
    resb 4096
p3_table: ;the Page-Directory Pointer Table (PDP),
    resb 4096
p2_table: ;the Page-Directory Table (PD),
    resb 4096
;and the Page Table (PT).
stack_bottom:
    resb 4 * 4096 ; orig is 4 *
stack_top:

#pragma once

#include "output.hpp"

namespace tiny_os {
namespace multiboot {

#ifndef MULTIBOOT_HEADERS
#define MULTIBOOT_HEADERS

#define MULTIBOOT_SEARCH 32768

#define MULTIBOOT_HEADER_ALIGN 8

#define MULTIBOOT_BOOTLOADER_MAGIC 0x36D76289

#define MULTIBOOT_HEADER_MAGIC 0xE85250D6

#define MULTIBOOT_UNSUPPORTED_FLAG_BITS 0x0000fffc

#define MULTIBOOT_MODULE_ALIGN 0x00001000

#define MULTIBOOT_INFO_STRUCT_ALIGN 0x00000008

#define MULTIBOOT_TAG_ALIGN 8
#define MULTIBOOT_TAG_TYPE_END 0
#define MULTIBOOT_TAG_TYPE_COMMANDLINE 1
#define MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME 2
#define MULTIBOOT_TAG_TYPE_MODULE 3
#define MULTIBOOT_TAG_TYPE_BASIC_MEMINFO 4
#define MULTIBOOT_TAG_TYPE_BOOTDEV 5
#define MULTIBOOT_TAG_TYPE_MMAP 6
#define MULTIBOOT_TAG_TYPE_VBE 7
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER 8
#define MULTIBOOT_TAG_TYPE_ELF_SECTIONS 9
#define MULTIBOOT_TAG_TYPE_APM 10

#define MULTIBOOT_HEADER_TAG_END 0
#define MULTIBOOT_HEADER_TAG_INFORMATION_REQUEST 1
#define MULTIBOOT_HEADER_TAG_ADDRESS 2
#define MULTIBOOT_HEADER_TAG_ENTRY_ADDRESS 3
#define MULTIBOOT_HEADER_TAG_CONSOLE_FLAGS 4
#define MULTIBOOT_HEADER_TAG_FRAMEBUFFER 5
#define MULTIBOOT_HEADER_TAG_MODULE_ALIGN 6
#define MULTIBOOT_ARCHITECTURE_I386 0
#define MULTIBOOT_ARCHITECTURE_MIPS32 4
#define MULTIBOOT_HEADER_TAG_OPTIONAL 1
#define MULTIBOOT_CONSOLE_FLAGS_CONSOLE_REQUIRED 1
#define MULTIBOOT_CONSOLE_FLAGS_EGA_TEXT_SUPPORTED 2

//flag checks
#define MEMORY_RANGE_FLAG_SET(flags) IS_BIT_SET(flags, 0)
#define BOOT_DEVICE_FLAG_SET(flags) IS_BIT_SET(flags, 1)
#define COMMANDLINE_FLAG_SET(flags) IS_BIT_SET(flags, 2)
#define BOOT_MODULES_FLAG_SET(flags) IS_BIT_SET(flags, 3)
#define SYMBOL_TABLE_FLAG_SET(flags) IS_BIT_SET(flags, 4)
#define ELF_SECTION_TABLE_FLAG_SET(flags) IS_BIT_SET(flags, 5)
#define MEMORY_MAP_FLAG_SET(flags) IS_BIT_SET(flags, 6)
#define DRIVES_FLAG_SET(flags) IS_BIT_SET(flags, 7)
#define CONFIG_TABLE_FLAG_SET(flags) IS_BIT_SET(flags, 8)
#define BOOT_LOADER_FLAG_SET(flags) IS_BIT_SET(flags, 9)
#define APM_TABLE_FLAG_SET(flags) IS_BIT_SET(flags, 10)
#define GRAPHICS_TABLE_FLAG_SET(flags) IS_BIT_SET(flags, 11)

#define MULTIBOOT_TAG_INIT(address) (multiboot::tag*) (address + 8)
#define MULTIBOOT_TAG_NEXT(tag) (multiboot::tag*)((unsigned char *) tag + ((tag->size + 7) & ~7))
#define MULTIBOOT_PRINT_TAG_INFO(str, tag) PRINT("tag type %s has size 0x%x\n", str, tag->size);
#define MULTIBOOT_PRINT_TAG_INFO_WITH_NAME(str, tag) PRINT("tag type %s has size 0x%x and name %s\n", str, tag->size, ((multiboot::tag_string *) tag)->string);

#endif

struct header {
	/* Must be MULTIBOOT MAGIC - see above. */
	unsigned int magic;
	/* ISA */
	unsigned int architecture;
	/* Total header length. */
	unsigned int header_length;
	/* The above fields plus this one must equal 0 mod 2^32. */
	unsigned int checksum;
};

struct header_tag {
	unsigned short type;
	unsigned short flags;
	unsigned int size;
};

struct header_tag_information_request {
	unsigned short type;
	unsigned short flags;
	unsigned int size;
	unsigned int requests[0];
};

struct header_tag_address {
	unsigned short type;
	unsigned short flags;
	unsigned int size;
	unsigned int header_address;
	unsigned int load_address;
	unsigned int load_end_address;
	unsigned int bss_end_address;
};

struct header_tag_entry_address {
	unsigned short type;
	unsigned short flags;
	unsigned int size;
	unsigned int entry_address;
};

struct header_tag_console_flags {
	unsigned short type;
	unsigned short flags;
	unsigned int size;
	unsigned int console_flags;
};

struct header_tag_framebuffer {
	unsigned short type;
	unsigned short flags;
	unsigned int size;
	unsigned int width;
	unsigned int height;
	unsigned int depth;
};

struct header_tag_module_align {
	unsigned short type;
	unsigned short flags;
	unsigned int size;
	unsigned int width;
	unsigned int height;
	unsigned int depth;
};

struct color {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

struct memory_map_entry {
	unsigned long long address;
	unsigned long long length;
#define MULTIBOOT_MEMORY_AVAILABLE 1
#define MULTIBOOT_MEMORY_RESERVED 2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT_MEMORY_NVS 4
	unsigned int type;
	unsigned int zero;
} __attribute__((packed));

struct tag {
	unsigned int type;
	unsigned int size;
};

struct tag_string {
	unsigned int type;
	unsigned int size;
	char string[0];
};

struct tag_module {
	unsigned int type;
	unsigned int size;
	unsigned int module_start;
	unsigned int module_end;
	char commandline[0];
};

struct tag_basic_meminfo {
	unsigned int type;
	unsigned int size;
	unsigned int memory_lower_bound;
	unsigned int memory_upper_bound;
};

struct tag_bootdev {
	unsigned int type;
	unsigned int size;
	unsigned int biosdev;
	unsigned int slice;
	unsigned int part;
};

struct tag_memory_map {
	unsigned int type;
	unsigned int size;
	unsigned int entry_size;
	unsigned int entry_version;
	memory_map_entry entries[0];
};

struct memory_map_iterator {
	tag_memory_map* ptr;
	memory_map_entry* current;

	memory_map_iterator(tag_memory_map* ptr) : ptr(ptr), current(((tag_memory_map*) ptr)->entries) {}

	void next() {
		do {
			auto start = (unsigned char*) current;
			auto end = ((unsigned char*) ptr) + (ptr->size);
			auto size = ptr->entry_size;

			if (start > end) {
				return;
			} else {
				auto entry = (memory_map_entry*) ((unsigned long)start + size);
				//if (entry->type > 0) PRINT("%x,", auto type = entry->type);
				if (entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
					current = entry;
					return;
				}
			}
		} while (true);
	}

	memory_map_iterator& operator++() { next(); return *this; }
	memory_map_iterator operator++(int) { auto temp = *this; next(); return temp; }

	operator bool() const { return (bool) current; }

	memory_map_entry* operator->() { return current; }
	memory_map_entry* operator*() { return current; }
};

struct vbe_info_block {
	unsigned char external_specification[512];
};

struct vbe_mode_info_block {
	unsigned char external_specification[256];
};

struct tag_vbe {
	unsigned int type;
	unsigned int size;
	unsigned short vbe_mode;
	unsigned short vbe_interface_seg;
	unsigned short vbe_interface_off;
	unsigned short vbe_interface_len;
	struct vbe_info_block vbe_control_info;
	struct vbe_mode_info_block vbe_mode_info;
};

struct tag_framebuffer_common {
	unsigned int type;
	unsigned int size;
	unsigned long long framebuffer_address;
	unsigned int framebuffer_pitch;
	unsigned int framebuffer_width;
	unsigned int framebuffer_height;
	unsigned char framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB 1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT 2
	unsigned char framebuffer_type;
	unsigned short reserved;
};

struct tag_framebuffer {
	struct tag_framebuffer_common common;
	union {
		struct {
			unsigned short framebuffer_palette_num_colors;
			color framebuffer_palette[0];
		};
		struct {
			unsigned char framebuffer_red_field_position;
			unsigned char framebuffer_red_mask_size;
			unsigned char framebuffer_green_field_position;
			unsigned char framebuffer_green_mask_size;
			unsigned char framebuffer_blue_field_position;
			unsigned char framebuffer_blue_mask_size;
		};
	};
};

struct tag_elf_sections {
	unsigned int type;
	unsigned int size;
	unsigned int num;
	unsigned int entsize;
	unsigned int shndx;
	char sections[0];
};

struct tag_apm {
	unsigned int type;
	unsigned int size;
	unsigned short version;
	unsigned short cseg;
	unsigned int offset;
	unsigned short cseg_16;
	unsigned short dseg;
	unsigned short flags;
	unsigned short cseg_len;
	unsigned short cseg_16_len;
	unsigned short dseg_len;
};

void print_info(size_type address) {
	io::console::printf("size of mbi 0x%x\n", *(unsigned long*) address);
	for (auto tag = MULTIBOOT_TAG_INIT(address); tag->type != MULTIBOOT_TAG_TYPE_END; tag = MULTIBOOT_TAG_NEXT(tag)) {
		switch (tag->type) {
			case MULTIBOOT_TAG_TYPE_COMMANDLINE:
				{
					MULTIBOOT_PRINT_TAG_INFO_WITH_NAME("commandline", tag);
				}
				break;
			case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
				{
					MULTIBOOT_PRINT_TAG_INFO_WITH_NAME("boot loader name", tag);
				}
				break;
			case MULTIBOOT_TAG_TYPE_MODULE:
				{
					MULTIBOOT_PRINT_TAG_INFO("module", tag);
					io::console::printf("module at 0x%x to 0x%x. command line %s\n", ((tag_module *) tag)->module_start, ((tag_module *) tag)->module_end, ((tag_module *) tag)->commandline);
				}
				break;
			case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
				{
					MULTIBOOT_PRINT_TAG_INFO("meminfo", tag);
					unsigned int memory_lower_bound = ((tag_basic_meminfo *) tag)->memory_lower_bound;
					unsigned int memory_upper_bound = ((tag_basic_meminfo *) tag)->memory_upper_bound;
					io::console::printf("memory_lower_bound = %uKB, memory_upper_bound = %uKB\n", memory_lower_bound, memory_upper_bound);
				}
				break;
			case MULTIBOOT_TAG_TYPE_BOOTDEV:
				{
					MULTIBOOT_PRINT_TAG_INFO("bootdev", tag);
					io::console::printf("boot device = 0x%x, slice = %u, part = %u\n", ((tag_bootdev *) tag)->biosdev, ((tag_bootdev *) tag)->slice, ((tag_bootdev *) tag)->part);
				}
				break;
			case MULTIBOOT_TAG_TYPE_MMAP:
				{
					MULTIBOOT_PRINT_TAG_INFO("memory map", tag);
					for (memory_map_entry * mmap = ((tag_memory_map *)tag)->entries; (unsigned char *)mmap < (unsigned char *)tag + tag->size; mmap = (memory_map_entry*)((unsigned long)mmap + ((tag_memory_map *)tag)->entry_size)) {
						io::console::printf(" base_addr = 0x%x%x, length = 0x%x%x, type = 0x%x\n", (unsigned)(mmap->address >> 32), (unsigned)(mmap->address & 0xffffffff), (unsigned)(mmap->length >> 32), (unsigned)(mmap->length & 0xffffffff), (unsigned)mmap->type);
					}
				}
				break;
			case MULTIBOOT_TAG_TYPE_VBE:
				{

				}
				break;
			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
				{
					// unsigned long color;
					// tag_framebuffer* tagfb = (tag_framebuffer*) tag;
					// void* frame_buffer = (void *) (unsigned long) tagfb->common.framebuffer_address;
					// switch (tagfb->common.framebuffer_type) {
					//	 case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
					//		 {
					//			 unsigned best_distance, distance;
					//			 struct color* palette = tagfb->framebuffer_palette;
					//			 color = 0;
					//			 best_distance = 4 * 256 * 256;
					//			 for (auto i = 0; i < tagfb->framebuffer_palette_num_colors; ++i)
					//			 {
					//				 distance = (0xff - palette[i].blue) * (0xff - palette[i].blue) + palette[i].red * palette[i].red + palette[i].green * palette[i].green;
					//				 if (distance < best_distance) {
					//					 color = i;
					//					 best_distance = distance;
					//				 }
					//			 }
					//		 }
					//		 break;
					//	 case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
					//		 color = ((1 << tagfb->framebuffer_blue_mask_size) - 1) << tagfb->framebuffer_blue_field_position;
					//		 break;
					//	 case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
					//		 color = '\\' | 0x0100;
					//		 break;
					//	 default:
					//		 color = 0xffffffff;
					//		 break;
					// }
					// for (auto i = 0; i < tagfb->common.framebuffer_width && i < tagfb->common.framebuffer_height; ++i) {
					//	 switch (tagfb->common.framebuffer_bpp) {
					//		 case 8:
					//			 {
					//				 unsigned char* pixel = (unsigned char*) (((size_type) frame_buffer) + tagfb->common.framebuffer_pitch * i + i);
					//				 *pixel = color;
					//			 }
					//		 break;
					//		 case 15:
					//		 case 16:
					//			 {
					//				 unsigned short* pixel = (unsigned short*) (((size_type) frame_buffer) + tagfb->common.framebuffer_pitch * i + 2 * i);
					//				 *pixel = color;
					//			 }
					//		 break;
					//		 case 24:
					//			 {
					//				 unsigned long* pixel = (unsigned long*) (((size_type) frame_buffer) + tagfb->common.framebuffer_pitch * i + 3 * i);
					//				 *pixel = (color & 0xffffff) | (*pixel & 0xff000000);
					//			 }
					//		 break;
					//		 case 32:
					//			 {
					//				 unsigned long* pixel = (unsigned long*) (((size_type) frame_buffer) + tagfb->common.framebuffer_pitch * i + 4 * i);
					//				 *pixel = color;
					//			 }
					//		 break;
					//	 }
					// }
				}
				break;
			case MULTIBOOT_TAG_TYPE_ELF_SECTIONS:
				{

				}
			case MULTIBOOT_TAG_TYPE_APM:
				{

				}
			default:
				break;
		}
	}
}

} // namespace multiboot
} // namespace tiny os

#pragma once

#include "utility.hpp"
#include "output.hpp"

namespace tiny_os {
namespace memory {

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096u
#endif

#ifndef ENTRY_COUNT
#define ENTRY_COUNT 512u
#endif

typedef size_type physical_address_type;
typedef size_type virtual_address_type;

// want to have allocator classes that wrap memory allocation logic,
// then we can have instances of allocators that manage different memory,
// instead of having global markers for memory is or is not

// for the global allocators, however, we must find memory locations for them
// which means segmenting (sizeof allocator * how many I want)

struct memory_node {
	memory_node* prev;
	size_type size;
	memory_node* next_free;
	memory_node* prev_free;
};

[[maybe_unused]] static const size_type memory_node_size = sizeof(memory_node);
static const size_type memory_node_user_space_offset = offsetof(memory_node, size) + sizeof(size_type);


#ifndef NODE_ADDRESS
#define _NODE_ADDRESS(node_pointer) ((size_type) node_pointer)
#define NODE_ADDRESS(node_pointer) _NODE_ADDRESS(node_pointer)
#endif

#ifndef NODE_USER_ADDRESS
#define _NODE_USER_ADDRESS(node_pointer) (NODE_ADDRESS(node_pointer) + memory_node_user_space_offset);
#define NODE_USER_ADDRESS(node_pointer) _NODE_USER_ADDRESS(node_pointer)
#endif

#ifndef NODE_TO_POINTER
#define _NODE_TO_POINTER_WITH_OFFSET(node_pointer, offset) ((void*) (NODE_ADDRESS(node_pointer) + offset))
#define NODE_TO_POINTER_WITH_OFFSET(node_pointer, offset) _NODE_TO_POINTER_WITH_OFFSET(node_pointer, offset)
#define NODE_TO_POINTER(node_pointer) NODE_TO_POINTER_WITH_OFFSET(node_pointer, memory_node_user_space_offset)
#endif

#ifndef LOCATION_TO_NODE
#define _POINTER_TO_NODE_WITH_OFFSET(pointer, offset) ((memory_node*) (NODE_ADDRESS(pointer) - offset))
#define LOCATION_TO_NODE_WITH_OFFSET(pointer, offset) _POINTER_TO_NODE_WITH_OFFSET(pointer, offset)
#define LOCATION_TO_NODE(pointer) LOCATION_TO_NODE_WITH_OFFSET(pointer, memory_node_user_space_offset);
#endif

memory_node* next_memory_node(const memory_node* const node) { return (memory_node*) (((size_type) node) + node->size); }

memory_node* new_memory_node(size_type begin, size_type size, memory_node* prev, memory_node* prev_free, memory_node* next_free) {
	memory_node* node = (memory_node*) (begin);
	node->size = size;
	node->prev = prev;
	node->prev_free = prev_free;
	node->next_free = next_free;
	return node;
}

memory_node* init_memory_node_head(const size_type& begin, const size_type& end) {
	memory_node* node = (memory_node*) (begin);
	node->size = 0;
	node->prev = nullptr;
	node->prev_free = nullptr;
	auto it = begin + memory_node_user_space_offset;
	memory_node* current = node;
	auto first_after_sentinel = true; // don't want to point back to sentinel and mess things up
	auto frames = 0;
	while (it < end) {
		auto size = (it + PAGE_SIZE < end) ? PAGE_SIZE : end - it;
		current->next_free = new_memory_node(it, size, current, nullptr, nullptr);
		current->next_free->prev_free = first_after_sentinel ? nullptr : current;
		current->next_free->prev = current;
		current = current->next_free;
		it += size + memory_node_user_space_offset;
		frames++;
		first_after_sentinel = false;
	}
	io::console::printf("start 0x%x, end 0x%x, frames %d \n", begin, end, frames);

	return node;
}

bool coalesce(memory_node* node) {
	auto next = next_memory_node(node);
	if (next && next->size/* && (node->size + next->size) <= PAGE_SIZE*/) { // size > 0 means for sale
		node->size += next->size;
		node->next_free = next->next_free;
		next->next_free->prev_free = node->prev_free;
		return true;
	}
	return false;
}

bool disperse(memory_node* node, size_type size) {
	long long disperse_size = node->size - size - memory_node_user_space_offset;
	if (disperse_size < 0) {
		return false;
	}
	size_type location = NODE_USER_ADDRESS(node) + size;
	node->next_free = new_memory_node(location, disperse_size, node, node, node->next_free);
	node->next_free->next_free->prev_free = node->next_free;
	return true;
}

// template <typename T>
// T* buy_memory(memory_node* head, size_type size) {
//	 memory_node* node = head->next_free;
//	 size_type node_address = (size_type) node;
//	 size_type bytes_to_allocate = sizeof(T) * size;
//	 if (node_address + bytes_to_allocate < end_of_memory) { // can allocate
//	 memory_node* current = node;
//	 while (bytes_to_allocate > node->size) { // may need to coalesce some nodes
//	 memory_node* next = next_memory_node(current);
//	 if (next->size) { // if there is size then we can eat, otherwise need to return nullptr
//	 coalesce(current);
//	 } else {
//	 return nullptr;
//	 }
//	 }
//	 T* res = (T*) NODE_TO_POINTER(head->next_free, memory_node_user_space_offset);
//	 }
// }

// template <typename T>
// T* malloc(size_type size) { return buy_memory<T>(size); }

union entry_flag { // should be aligned and have size of 8
	struct {
		size_type present : 1;
		size_type writable : 1;
		size_type user_accessible : 1;
		size_type write_through_caching : 1;
		size_type disable_cache : 1;
		size_type accessed : 1;
		size_type dirty : 1;
		size_type huge_page_or_null : 1;
		size_type global : 1;
		size_type os_available_small : 3;
		size_type physical_address : 40;
		size_type os_available_large : 11;
		size_type no_execute : 1;
	};
	size_type flag;
};

entry_flag page_table[ENTRY_COUNT];

[[maybe_unused]] static const size_type p4_table = 0177777777777777770000;

void initialize_page_table() {
	for (auto i = 0u; i < ENTRY_COUNT; ++i) {
		page_table[i].flag = 0;
	}
}

struct page_table {};
struct page_table_1 : page_table {};
struct accessible_page_table : page_table {};
struct page_table_2 : accessible_page_table {};
struct page_table_3 : accessible_page_table {};
struct page_table_4 : accessible_page_table {};

bool page_is_present(entry_flag entry) { return entry.present; }
bool page_is_writable(entry_flag entry) { return entry.writable; }
bool page_is_user_accessible(entry_flag entry) { return entry.user_accessible; }
bool page_is_write_through_caching(entry_flag entry) { return entry.write_through_caching; }
bool page_is_disable_cache(entry_flag entry) { return entry.disable_cache; }
bool page_is_accessed(entry_flag entry) { return entry.accessed; }
bool page_is_dirty(entry_flag entry) { return entry.dirty; }
bool page_is_huge_page_or_null(entry_flag entry) { return entry.huge_page_or_null; }
bool page_is_global(entry_flag entry) { return entry.global; }
size_type page_os_available_small(entry_flag entry) { return entry.os_available_small; }
size_type page_physical_address(entry_flag entry) { return entry.physical_address; }
size_type page_os_available_large(entry_flag entry) { return entry.os_available_large; }
bool page_is_no_execute(entry_flag entry) { return entry.no_execute; }
//optional<frame> frame_from_entry(entry_flag entry) { return optional<frame>(page_physical_address(entry)); }
// size_type next_table_address(size_type index) {
//	 auto entry = page_table[index];
//	 if (page_is_present(entry) && !page_is_huge_page_or_null(entry)) {
//	 return
//	 }
// }

template <typename T>
class pointer {
public:
	typedef T type;
	typedef T* pointer_type;
protected:
private:
	T* _ptr;
public:
	pointer(size_type size);// { _ptr = buy_memory<T>(size); }
	pointer(const pointer&) = default;
	pointer(pointer&&) = default;
	~pointer() {	}

	pointer& operator=(const pointer&) = default;
	pointer& operator=(pointer&&) = default;
protected:
private:
};

template <typename T>
class allocator {
public:
	typedef T type;
	typedef T* pointer_type;
	typedef memory_node* memory_node_pointer_type;
protected:
private:
	size_type _begin, _end;
	memory_node_pointer_type _head;
public:
	allocator(size_type begin, size_type end) : _begin(begin), _end(end) { _head = init_memory_node_head(_begin, _end); }
	allocator(const allocator&) = default;
	allocator(allocator&&) = default;
	~allocator() = default;

	allocator& operator=(const allocator&) = default;
	allocator& operator=(allocator&&) = default;

	pointer_type allocate(size_type size) { return _buy_memory(_head, size); }
	void deallocate(pointer_type ptr) { _sell_memory(ptr); }
protected:
private:
	T* _buy_memory(memory_node* head, const size_type size) {
		memory_node* node = head->next_free;
		size_type node_address = (size_type) node;
		const size_type bytes_to_allocate = sizeof(T) * size;
		if (node_address + bytes_to_allocate < _end) { // can allocate
			memory_node* current = node;
			while (bytes_to_allocate > node->size) { // may need to coalesce some nodes
				memory_node* next = next_memory_node(current);
				if (next->size) { // if there is size then we can eat, otherwise need to return nullptr
					coalesce(current);
				} else {
					return nullptr;
				}
			}
			if (size < current->size) {
				disperse(current, size);
			}
			T* res = (T*) NODE_TO_POINTER(current);
			head->next_free = current->next_free;
			current->next_free->prev_free = current->prev_free;
			return res;
		}
		return nullptr;
	}

	void _sell_memory(T* ptr) {
		auto node = LOCATION_TO_NODE(ptr);
		if (node) {
			auto next_free = _head->next_free;
			while (next_free && (NODE_ADDRESS(next_free) < NODE_ADDRESS(node))) next_free = next_free->next_free;
			auto prev_free = next_free->prev_free;
			node->next_free = next_free;
			node->prev_free = prev_free;
			while (node->size < PAGE_SIZE && node->next_free == next_memory_node(node)) coalesce(node);
		}
	}
};

} // namespace memory
} // namespace tiny os

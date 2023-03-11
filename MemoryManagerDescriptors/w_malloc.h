//https://www.youtube.com/watch?v=xrJfaLBMu2w&list=PLR4wcBxrUGPBED2vUPaPY0Eo__ba1LJGC&index=4

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <string>
#include <forward_list>

using namespace std;

#define u8 uint8_t
#define u16 uint16_t
#define STACK_SIZE 128
#define HEAP_SIZE STACK_SIZE * 4  // этот параметр отвечает за количество доступной памяти
#define HEADER 4

static u16 IN_USE = 0;

static u16 ENGAGED = 0;

typedef struct virtual_memory
{
	u8 heap[HEAP_SIZE];

}virtual_memory_t;


struct entity
{
    u8* ptr = nullptr;
    int size = 0;

};

struct Obj {
    u8* ptr = nullptr;
    int refCount = 0;
};


forward_list<entity> LIST;


void LOG()
{

    int free_size = 0;
    int biggest_free_size = 0;
    for (auto ent : LIST)
	{
        free_size += ent.size;
        if (biggest_free_size < ent.size)
            biggest_free_size = ent.size;

    }

    if ((LIST.empty() and !ENGAGED)) {
         free_size = HEAP_SIZE;
         biggest_free_size = HEAP_SIZE;
    }
    printf("Free memory size: [%d]\n", free_size);
    printf("Size of the largest free block: [%d]\n", biggest_free_size);
    printf("Amount of blocks used: [%d]\n", ENGAGED);
}


entity* new_entity(int size)
{
    if (LIST.empty() and !ENGAGED) {
        static virtual_memory_t vm;
        entity first;
        first.ptr = vm.heap;
        first.size = HEAP_SIZE;
        LIST.push_front(first);
        IN_USE++;
    }


    entity* best = nullptr;

    auto iter = LIST.before_begin();

    for (auto ent = LIST.begin(); ent != LIST.end(); ent++)  // added
	{
        if ((*ent).size >= size) {
            if ((*ent).size == size) {
                LIST.erase_after(iter);
            }
            best = &*ent;
            break;
        }
        iter = ent;
	}

    return best;
}

void* w_malloc(int size)
{

	size += HEADER;

    if (size > HEAP_SIZE or size < HEADER + 1)
        return nullptr;

    entity* e = new_entity(size);

    if (e == nullptr)
        return e;

    ENGAGED++;

	u8* start = e->ptr;
	u8* user_ptr = start + HEADER;

    *(int*)start = size;

	e->ptr += size;
	e->size -= size;

	return user_ptr;
}


void w_free(void* ptr)
{
	u8* start = (u8*)ptr - HEADER;

    ENGAGED--;

    entity freed_ent;

    int size = *(int*)start;

    freed_ent.ptr = start;
    freed_ent.size = size;

    auto q = LIST.before_begin();

    for (auto p = LIST.begin(); ; p++) {
        if (p == LIST.end() or (*p).ptr > freed_ent.ptr) {
            if (p != LIST.end()) {
                if (start + size == (*p).ptr) {
                    size += (*p).size;
                    LIST.erase_after(q);
                }
            }
            if (q != LIST.before_begin()) {
                if ((*q).ptr + (*q).size == start) {
                    (*q).size += size;
                    *(*q).ptr = size;
                    break;
                }
            }

            *freed_ent.ptr = size;
            freed_ent.size = size;
            LIST.emplace_after(q, freed_ent);

            break;

        }

        q++;
    }
}





void delete_all_memory() {
    LIST.clear();
    IN_USE = 0;
    ENGAGED = 0;
}

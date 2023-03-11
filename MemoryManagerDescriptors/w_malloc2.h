#ifndef W_MALLOC2_H
#define W_MALLOC2_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
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




struct Obj {
    u8* ptr = nullptr;
    int refCount = 0;
};

static virtual_memory_t vm;
static u8* avail = nullptr;

void LOG()
{

    int free_size = 0;
    int biggest_free_size = 0;

    u8* runner = avail;

    while (runner != nullptr)
    {
        int size = *(int*)runner;
        free_size += size;
        if (biggest_free_size < size)
            biggest_free_size = size;
        runner = *(u8**)(runner + sizeof(int));

    }

    if (!IN_USE and !ENGAGED) {
         free_size = HEAP_SIZE;
         biggest_free_size = HEAP_SIZE;
    }

    printf("Free memory size: [%d]\n", free_size);
    printf("Size of the largest free block: [%d]\n", biggest_free_size);
    printf("Amount of blocks used: [%d]\n", ENGAGED);
}

u8* new_entity(int size)
{
    if (size > HEAP_SIZE)
        return nullptr;

    if (!IN_USE and !ENGAGED) {
        *(int*)(vm.heap) = HEAP_SIZE;
        *(int*)(vm.heap + HEAP_SIZE - sizeof(int)) = HEAP_SIZE;
        *(u8**)(vm.heap + sizeof(int)) = nullptr;
        *(u8**)(vm.heap + sizeof(int) + sizeof(u8*)) = nullptr;
        avail = vm.heap;
        IN_USE++;
    }

    u8* best = nullptr;
    u8* next = avail;

    while (next != nullptr)  // added
    {
        best = next;
        if (*(int*)best >= size) {
            break;
        }
        next = *(u8**)(next + sizeof(int));
    }

    if (next == nullptr) {
        return nullptr;
    }

    return best;
}

void* w_malloc(int size)
{
    size += 2 * sizeof(int) + 2*sizeof(u8*);

    if (size > HEAP_SIZE)
        return nullptr;

    u8* e = new_entity(size);

    if (e == nullptr)
        return nullptr;

    ENGAGED++;

    u8* start = e;
    u8* user_ptr = start + sizeof(int);

    int free_size = *(int*)start - size;

    u8* next = *(u8**)(start + sizeof(int));
    u8* prev = *(u8**)(start + sizeof(int) + sizeof(u8*));

    if (free_size <= (int)(1 + 2*sizeof(int) + 2*sizeof(u8*))) {
	if (avail == start) {
       	    avail = next;
	}

        size = *(int*)start;

        if (prev != nullptr) {
            *(u8**)(prev + sizeof(int)) = next;
        }
        if (next != nullptr) {
            *(u8**)(next + sizeof(int) + sizeof(u8*)) = prev;
        }
    }
    else {
        if (avail == start) {
            avail = avail + size;
        }
        *(int*)(start + size) = free_size;
        *(int*)(start + free_size + size - sizeof(int)) = free_size;

        *(u8**)(start + size + sizeof(int)) = next;
        *(u8**)(start + size + sizeof(int) + sizeof(u8*)) = prev;
    }
    *(int*)(start + size - sizeof(int)) = -size;
    *(int*)start = -size;

    return user_ptr;
}

void w_free(void *ptr) {
    //  у нас 1 список, в блоке 2 хэдера, которые состоят из размера и указателя на следующий/предыдущий свободный
    u8* start = (u8*)ptr - HEADER;
    ENGAGED--;
    *(int*)start = -(*(int*)start);  // отрицательный т.к. блок был занят (- (-Х))
    int size = *(int*)start;
    u8 *prev, *post = start + size;
    if (start == vm.heap) {
        prev = start - 1;
    }
    else {
        prev = start - *(int*)(start - sizeof(int));
    }

    if (*(int*)(prev) > 0 and prev >= vm.heap) {  // проверка нижнего соседнего блока
        size += *(int*)prev;
        *(int*)prev = size;
        *(int*)(start + *((int*)start) - sizeof(int)) = size;
        start = prev;
    }
    if (*(int*)(post) > 0 and post < vm.heap + HEAP_SIZE) {  // проверка верхнего соседнего блока
        size += *(int*)post;
        *(int*)(start + size - sizeof(int)) = size;
        *(int*)start = size;

        if (prev != start) {
            *(u8**)(start + sizeof(int) + sizeof(u8*)) = *(u8**)(post + sizeof(int) + sizeof(u8*));  // указатель на предыдущий
            u8* rel_prev = *(u8**)(post + sizeof(int) + sizeof(u8*));
            if (rel_prev != nullptr) {
                *(u8**)(rel_prev + sizeof(int)) = start;
            }
        }
        *(u8**)(start + sizeof(int)) = *(u8**)(post + sizeof(int));  //указатель на cледующий
        if (avail == post) {
            avail = start;
        }
        post = start;
    }

    if (start != post and start != prev) {

        IN_USE++;

        post = avail;
        prev = *(u8**)(post + sizeof(int) + sizeof(u8*));

        while(post < start and post != nullptr) {
            prev = post;
            post = *(u8**)(post + sizeof(int));
        }

        if (post != nullptr) {
            *(u8**)(post + sizeof(int) + sizeof(u8*)) = start;
        }
        *(u8**)(start + sizeof(int)) = post;

        if (prev != nullptr) {
            *(u8**)(prev + sizeof(int)) = start;
        }
        *(u8**)(start + sizeof(int) + sizeof(u8*)) = prev;

        if (prev == nullptr) {
            avail = start;
        }

        *(int*)(start + size - sizeof(int)) = size;
    }

}


void delete_all_memory() {
    IN_USE = 0;
    ENGAGED = 0;
}

#endif // W_MALLOC2_H

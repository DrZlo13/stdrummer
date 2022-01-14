#include <cmsis_os.h>
#include <string.h>

extern "C" {
void* malloc(size_t size) {
    return pvPortMalloc(size);
}

void free(void* ptr) {
    vPortFree(ptr);
}

void* realloc(void* ptr, size_t size) {
    if(size == 0) {
        vPortFree(ptr);
        return NULL;
    }

    void* p;
    p = pvPortMalloc(size);
    if(p) {
        // TODO implement secure realloc
        // insecure, but will do job in our case
        if(ptr != NULL) {
            memcpy(p, ptr, size);
            vPortFree(ptr);
        }
    }
    return p;
}

void* calloc(size_t count, size_t size) {
    void* ptr = pvPortMalloc(count * size);
    if(ptr) {
        // zero the memory
        memset(ptr, 0, count * size);
    }
    return ptr;
}

size_t memmgr_get_free_heap(void) {
    return xPortGetFreeHeapSize();
}

size_t memmgr_get_minimum_free_heap(void) {
    return xPortGetMinimumEverFreeHeapSize();
}

void* __wrap__malloc_r(struct _reent* r, size_t size) {
    void* pointer = malloc(size);
    return pointer;
}

void __wrap__free_r(struct _reent* r, void* ptr) {
    free(ptr);
}

void* __wrap__calloc_r(struct _reent* r, size_t count, size_t size) {
    void* pointer = calloc(count, size);
    return pointer;
}

void* __wrap__realloc_r(struct _reent* r, void* ptr, size_t size) {
    void* pointer = realloc(ptr, size);
    return pointer;
}
}
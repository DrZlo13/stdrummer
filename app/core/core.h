#pragma once
#include <stdlib.h>

#ifdef __cplusplus
namespace Core {
void crash(const char* message = NULL);
void log_stack();
void log_heap();
}
#else

void core_crash(const char* message);

#endif

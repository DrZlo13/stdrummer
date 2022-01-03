#pragma once
#include <stdlib.h>

#ifdef __cplusplus

void core_crash(const char* message = NULL);

#else

void core_crash(const char* message);

#endif

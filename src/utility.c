#include "utility.h"

char* strndup(const char* src, size_t n) {
    char* dest = malloc(n + 1);
    if (!dest) return NULL;
    strncpy(dest, src, n);
    dest[n] = '\0';  // Ensure null-termination
    return dest;
}

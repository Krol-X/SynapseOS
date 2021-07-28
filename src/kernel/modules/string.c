#include "string.h"
#include <stddef.h>


void memset(void *mem, char value, size_t count) {
    asm("movl %0, %%eax \n movl %1, %%edi \n movl %2, %%ecx \n rep stosl"
        ::"a"((uint32)value | ((uint32)value << 8) | ((uint32)value << 16) | ((uint32)value << 24)),"b"(mem),"c"(count >> 2));
    asm("movb %b0, %%al \n movl %1, %%ecx \n rep stosb"::"a"(value),"b"(count & 3));
}


void memset_word(void *mem, uint16 value, size_t count) {
    asm("movl %0, %%eax \n movl %1, %%edi \n movl %2, %%ecx \n rep stosl"::"a"((uint32)value | ((uint32)value << 16)),"b"(mem),"c"(count >> 1));
}


void memcpy(void *dest, void *src, size_t count) {
    asm("movl %0, %%edi \n movl %1, %%esi \n movl %2, %%ecx \n rep movsl"::"a"(dest),"b"(src),"c"(count >> 2));
    asm("movl %0, %%ecx \n rep movsb"::"a"(count & 3));
}


int memcmp(const void *str1, const void *str2, size_t count) {
    const unsigned char *s1 = str1;
    const unsigned char *s2 = str2;
    while (count-- > 0){
      if (*s1++ != *s2++)
            return s1[-1] < s2[-1] ? -1 : 1;
    }
  return 0;
}


void *memchr(void *mem, char value, size_t count) {
    void *result;
    asm("movb %b0, %%al \n movl %1, %%edi \n movl %2, %%ecx \n repe cmpsb"::"a"(value),"b"(mem),"c"(count));
    asm("movl %%edi, %0":"=a"(result));
    if (result < mem + count) {
        return result;
    } else {
        return S_NULL;
    }
}


size_t strlen(char *str) {
    return (char*)memchr(str, '\0', -1) - str;
}


void strcpy(char *dest, char *src) {
    memcpy(dest, src, strlen(src) + 1);
}


void strncpy(char *dest, char *src, size_t max_count) {
    size_t len = min(max_count - 1, strlen(src));
    memcpy(dest, src, len);
    dest[len] = '\0';
}


int strcmp(char *str1, char *str2) {
    return memcmp(str1, str2, strlen(str1) + 1);
}


char *strchr(char *str, char value) {
    return memchr(str, value, strlen(str));
}
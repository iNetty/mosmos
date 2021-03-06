#ifndef _LIB_H_
#define _LIB_H_

void memcpy(void *_dst, void *_src, uint32_t size);
void memset(void *_dst, uint8_t val, uint32_t size);
void strncpy(char *dst, char *src, uint32_t size);
int strcmp(char *str1, char *str2);
int strncmp(char *str1, char *str2, uint32_t len);
uint32_t strlen(char *str);
void panic();
uint32_t max_uint32(uint32_t num1, uint32_t num2);
uint32_t min_uint32(uint32_t num1, uint32_t num2);
int min_int(int num1, int num2);

#endif

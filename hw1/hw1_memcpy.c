/*
 * UCSC Embedded and Firmware Essential Course - HW1
 *
 * Implementing memcpy() function using C.
 *
 * Note: this program might have some issues depending
 * on the CPU architecture such as bye order definition.
 *
 * Author:  Hisakazu Ishiguro
 * Date:    Oct 8, 2014
 */

#include <stdio.h>
#include <assert.h>

void *memcpy(void *dest, const void *src, size_t n)
{
  int i;
  char *dst0;
  const char *src0;

  assert(dest != NULL);
  assert(src != NULL);
  assert(n > 0);

  if (dest <= src || (dest >= (src + n))) {
    dst0 = (char *)dest;
    src0 = (const char *)src;
    i = n;

    while (n--)
      *dst0++ = *src0++;

  } else {
    dst0 = (char *)(dest + n - 1);
    src0 = (const char *)(src + n - 1);
    i = n;

    while (n--)
      *dst0-- = *src0--;
  }

  return dest;
}

#define BUF_SZ 50
char buf[BUF_SZ];

void reset_buf()
{
  int i;
  char ascii = '0';

  for (i = 0; i < BUF_SZ; i++)
    buf[i] = ascii++;
}

void print_buf()
{
  int i;

  for (i = 0; i < BUF_SZ; i++)
    printf("%c", buf[i]);
  printf("\n");
}

void print_coordinate()
{
  int i;

  for (i = 0; i < BUF_SZ; i++) {
    if (!(i % 10))
      printf("%d", (i / 10));
    else printf("%c", ' ');
  }
  printf("\n");

  for (i = 0; i < BUF_SZ; i++)
    printf("%d", (i % 10));
  printf("\n");
}

void test_memcpy(int src_idx, int dst_idx, size_t sz)
{
  char *dst, *src;

  assert(sz <= BUF_SZ);
  assert((src_idx >= 0) && ((src_idx + sz - 1)< BUF_SZ));
  assert((dst_idx >= 0) && ((dst_idx + sz - 1)< BUF_SZ));

  reset_buf();
  
  src = &buf[src_idx];
  dst = &buf[dst_idx];

  printf("Test memcpy, src_idx=%d dst_idx=%d n=%d\n",
	 src_idx, dst_idx, (int)sz);

  memcpy((void *)dst, (const void *)src, sz);
  print_buf();
  printf("\n");
}

int main()
{
  printf("Initial Buf data:\n");
  print_coordinate();
  reset_buf();
  print_buf();
  printf("\n");

  // Start memcpy tests
  test_memcpy(0, 20, 5);     // no overlapped
  test_memcpy(20, 0, 5);     // no overlapped
  test_memcpy(20, 25, 10);   // overlapped (src < dst) @forward copy
  test_memcpy(30, 25, 10);   // overlapped (src > dst) @reverse copy
  test_memcpy(10, 10, 20);   // overlapped (src = dst) @forward copy
  test_memcpy(0, 40, (BUF_SZ - 40)); // no overlapped
  test_memcpy(0, 0, BUF_SZ); // critical test

  return 0;
}

#if 0
///////////////////////////////////////////////////////////////
EXECUTION OUTPUT
///////////////////////////////////////////////////////////////
$ date
Wed Oct  8 16:10:57 PDT 2014
$ arch
x86_64
$ gcc --version
gcc (Ubuntu 4.8.2-19ubuntu1) 4.8.2
Copyright (C) 2013 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

$ ls -lt *.c
-rw-rw-r-- 1 ishiguro ishiguro 2451 Oct  8 16:09 hw1_memcpy.c
$
$ gcc hw1_memcpy.c
$ ./a.out
Initial Buf data:
0         1         2         3         4         
01234567890123456789012345678901234567890123456789
0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`a

Test memcpy, src_idx=0 dst_idx=20 n=5
0123456789:;<=>?@ABC01234IJKLMNOPQRSTUVWXYZ[\]^_`a

Test memcpy, src_idx=20 dst_idx=0 n=5
DEFGH56789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`a

Test memcpy, src_idx=20 dst_idx=25 n=10
0123456789:;<=>?@ABCDEFGHDEFGHIJKLMSTUVWXYZ[\]^_`a

Test memcpy, src_idx=30 dst_idx=25 n=10
0123456789:;<=>?@ABCDEFGHNOPQRSTUVWSTUVWXYZ[\]^_`a

Test memcpy, src_idx=10 dst_idx=10 n=20
0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`a

Test memcpy, src_idx=0 dst_idx=40 n=10
0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVW0123456789

Test memcpy, src_idx=0 dst_idx=0 n=50
0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`a

$ 

#endif


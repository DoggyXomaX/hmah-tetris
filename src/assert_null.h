#ifndef __ASSERT_NULL_H__
#define __ASSERT_NULL_H__

#include <stdio.h>
#define assertNullReturn(x, return_value) if (this == NULL) { fprintf(stderr, "%s: " #x " is NULL!\n", __func__); return return_value; }
#define assertNull(x) assertNullReturn(x,)

#endif // __ASSERT_NULL_H__
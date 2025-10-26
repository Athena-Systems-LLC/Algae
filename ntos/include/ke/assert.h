/*
 * ALGAE assert macros
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_ASSERT_H_
#define _KE_ASSERT_H_ 1

#include <ke/bugCheck.h>

#define ASSERT(COND)                                                        \
    if ((USIZE)((COND)) == 0) {                                             \
        keBugCheck("assert \"%s\" failed (%s() at %s:%d)\n", #COND,         \
              __func__, __FILE__, __LINE__);                                \
    }

#endif  /* !_KE_ASSERT_H_ */

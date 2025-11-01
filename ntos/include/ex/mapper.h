/*
 * ALGAE executive mapper layer
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _EX_MAPPER_H_
#define _EX_MAPPER_H_

#include <ke/types.h>

/*
 * Represents valid mapper types
 *
 * @MAPPER_FIXED: Fixed mappings describe by a region descriptor
 * @MAPPER_DYNAMIC: Dynamic mappings describe by callbacks
 */
typedef enum {
    MAPPER_FIXED,
    MAPPER_DYNAMIC
} MAPPER_TYPE;

/*
 * Represents a mappable region
 *
 * @vBase: Virtual base address     [page aligned]
 * @pBase: Physical base address    [page aligned]
 * @length: Length of region        [page aligned]
 */
typedef struct {
    ULONG_PTR vBase;
    ULONG_PTR pBase;
    USIZE length;
} MAPPER_REGION;

/*
 * Represents a generic mapper object
 *
 * @type: Mapper type
 * @region: Used if type is MAPPER_FIXED
 */
typedef struct {
    MAPPER_TYPE type;
    MAPPER_REGION region;
} MAPPER_OBJECT;

#endif  /* !_EX_MAPPER_H_ */

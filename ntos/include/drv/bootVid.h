/*
 * ALGAE boot video driver interface
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _DRV_BOOTVID_H_
#define _DRV_BOOTVID_H_ 1

#include <ke/types.h>

/*
 * Clear the screen by filling it with an RGB pixel
 * described by `rgb'
 *
 * @rgb: RGB pixel to fill screen with
 */
void bootVidClear(ULONG rgb);

#endif  /* !_DRV_BOOTVID_H_ */

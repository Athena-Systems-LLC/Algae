/*
 * ALGAE C runtime initialization
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

extern int main(void);

int
__mainCrtStartup(void)
{
    return main();
}

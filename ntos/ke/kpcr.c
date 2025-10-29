#include <hal/kpcr.h>

struct kpcr *
keCpuGet(USIZE index)
{
    struct kpcr *kpcr = NULL;

    halCpuGet(index, &kpcr);
    return kpcr;
}

USIZE
keCpuCount(void)
{
    return halCpuCount();
}

#include <stdint.h>
#include <stdio.h>
#include "misc.h"

void trap()
{
    printf("Trapped.\n");
    while (1);
}

void register_wait_flags_set(__IO uint32_t *register_addr, uint16_t flags)
{
    while (!(*register_addr & flags));
}

void register_wait_flags_reset(__IO uint32_t *register_addr, uint16_t flags)
{
    while ((*register_addr & flags));
}

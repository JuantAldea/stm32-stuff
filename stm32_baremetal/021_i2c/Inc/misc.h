#ifndef MISC_H_
#define MISC_H_

#include <stm32f4xx.h>

void trap();

void register_wait_flags_set(__IO uint32_t *register_addr, uint16_t flags);

void register_wait_flags_reset(__IO uint32_t *register_addr, uint16_t flags);

#endif /* MISC_H_ */

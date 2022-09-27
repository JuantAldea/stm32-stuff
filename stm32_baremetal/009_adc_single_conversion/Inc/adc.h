#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void start_conversion(void);
uint32_t adc_read(void);
void pa1_adc_init(void);

#endif /* ADC_H_ */

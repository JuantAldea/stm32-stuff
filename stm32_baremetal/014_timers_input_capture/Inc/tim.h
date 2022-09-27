#ifndef TIM_H_
#define TIM_H_

//void tim2_1hz_init(void);
void tim3_1hz_init(void);
void tim3_output_compare_PA6(void);
void tim2_output_compare_PA5(void);
void tim2_input_capture_PA1(void);
void tim2_input_capture_PA0(void);
void tim2_input_capture_PA2_CH3(void);
void input_capture_PA0_TIM5_CH1(void);
void input_capture_PA7_TIM3_CH2(void);

void output_compare_PA5_TIM2_CH1(void);
void output_compare_PA6_TIM3_CH1(void);
void input_capture_PA5_TIM2_CH1(void);
void input_capture_PA6_TIM3_CH1(void);
void output_compare_PA7_TIM3_CH2(void);
void input_capture_PA7_TIM3_CH2(void);
void input_capture_PA2_TIM2_CH3(void);
void output_compare_PA2_TIM2_CH3(void);

void output_compare_PA0_TIM5_CH1(void);
void input_capture_PA0_TIM5_CH1(void);

#endif /* TIM_H_ */

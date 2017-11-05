//--------------------------------------------------------------
// File     : stm32_ub_button.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F10_UB_BUTTON_H
#define __STM32F10_UB_BUTTON_H

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"



//--------------------------------------------------------------
// ��������� � ���������� ������� ���������� �������� ���������
// 1 = �������, 0 = ���������
//--------------------------------------------------------------
#define  BUTTON_USE_TIMER      1  // ������� �� �������


#if BUTTON_USE_TIMER==1
  #include "stm32f10x_tim.h"
  #include "misc.h"
  #include <stdbool.h>
#endif


//--------------------------------------------------------------
// ������ ���� ������
// �� �����������, ��������� ���������� � 0
//--------------------------------------------------------------
typedef enum 
{
  BTN_1 = 0,    // BTN1 auf dem STM32F4-Discovery
  BTN_2 = 1    // BTN1 auf dem STM32F4-Discovery
}BUTTON_NAME_t;

#define  BUTTON_ANZ   2 // Anzahl von Button_NAME_t


//--------------------------------------------------------------
// ������ ������
//--------------------------------------------------------------
typedef enum {
  BTN_RELEASED = 0,  // ������ �� ������
  BTN_PRESSED        // ������ ������
}BUTTON_STATUS_t;



//--------------------------------------------------------------
// ������ ��������� ������ => TIM7
// ������� ������������ = 2*APB1 (APB1=42MHz) => TIM_CLK=84MHz
// TIM_Frq = TIM_CLK/������+1)/(��������+1)
// TIM_Frq = 20Hz => 50ms (��������������� �� ����� 1��)
//--------------------------------------------------------------
#if BUTTON_USE_TIMER==1
  #define   UB_BUTTON_TIM              TIM4
  #define   UB_BUTTON_TIM_CLK          RCC_APB1Periph_TIM4
  #define   UB_BUTTON_TIM_PERIODE      4999
  #define   UB_BUTTON_TIM_PRESCALE     839
  #define   UB_BUTTON_TIM_IRQ          TIM4_IRQn
  #define   UB_BUTTON_TIM_ISR_HANDLER  TIM4_IRQHandler
#endif



//--------------------------------------------------------------
// ��������� ���������������� ������
//--------------------------------------------------------------
typedef struct {
  BUTTON_NAME_t BUTTON_NAME;  // Name
  GPIO_TypeDef* BUTTON_PORT;  // Port
  const uint16_t BUTTON_PIN;  // Pin
  const uint32_t BUTTON_CLK;  // Clock
  GPIOMode_TypeDef BUTTON_R;  // Widerstand
  uint8_t BUTTON_AKT;         // Istwert
}BUTTON_t;


//--------------------------------------------------------------
// ���������� �������
//--------------------------------------------------------------
void UB_Button_Init(void);
BUTTON_STATUS_t UB_Button_Read(BUTTON_NAME_t btn_name);
#if BUTTON_USE_TIMER==1
  bool UB_Button_OnPressed(BUTTON_NAME_t btn_name);
  bool UB_Button_OnClick(BUTTON_NAME_t btn_name);
  bool UB_Button_OnRelease(BUTTON_NAME_t btn_name);
#endif

//--------------------------------------------------------------
#endif // __STM32F10_UB_BUTTON_H

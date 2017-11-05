//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_led.h"


//--------------------------------------------------------------
// ����������� ���� �����������
// ���������� ��� LED_NAME_t
//
// Init : [LED_OFF,LED_ON]
//--------------------------------------------------------------
LED_t LED[] = {
  // Name    ,PORT , PIN       , CLOCK              , Init
  {LED_DEBUG ,GPIOC,GPIO_Pin_13,RCC_APB2Periph_GPIOC,LED_OFF},   // PD12=������� ��������� �� ����� Discovery
};



//--------------------------------------------------------------
// ������������� ���� �����������
//--------------------------------------------------------------
void UB_Led_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  LED_NAME_t led_name;
  
  for(led_name=0;led_name<LED_ANZ;led_name++) {
    // ��������� �������������
    RCC_APB2PeriphClockCmd(LED[led_name].LED_CLK, ENABLE);

    // ������������ ����� ��� ��������� ������
    GPIO_InitStructure.GPIO_Pin = LED[led_name].LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED[led_name].LED_PORT, &GPIO_InitStructure);

    // ��������� �������� �� ���������
    if(LED[led_name].LED_INIT==LED_OFF) {
    	 UB_Led_Off(led_name);
    }
    else {
    	UB_Led_On(led_name);
    }
  }
}


//--------------------------------------------------------------
// ���������� ����������
//--------------------------------------------------------------
void UB_Led_Off(LED_NAME_t led_name)
{
  //LED[led_name].LED_PORT->BSRR =1<<LED[led_name].LED_PIN;
  GPIO_ResetBits(LED[led_name].LED_PORT,LED[led_name].LED_PIN);

}

//--------------------------------------------------------------
// ��������� ����������
//--------------------------------------------------------------
void UB_Led_On(LED_NAME_t led_name)
{
	 //LED[led_name].LED_PORT->BRR = 1<<LED[led_name].LED_PIN;
	 GPIO_SetBits(LED[led_name].LED_PORT,LED[led_name].LED_PIN);
} 

//--------------------------------------------------------------
// ����������� ���������
//--------------------------------------------------------------
void UB_Led_Toggle(LED_NAME_t led_name)
{
  LED[led_name].LED_PORT->ODR ^= LED[led_name].LED_PIN;
}

//--------------------------------------------------------------
// ������������� ��������� ����������
//--------------------------------------------------------------
void UB_Led_Switch(LED_NAME_t led_name, LED_STATUS_t wert)
{
  if(wert==LED_OFF) {
    UB_Led_Off(led_name);
  }
  else {
    UB_Led_On(led_name);
  }
}

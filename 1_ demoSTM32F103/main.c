#include "main.h"
#include "stm32_ub_led.h"
#include "st7735.h"
#include "stm32f10x_usart.h"
#include <stdio.h>

void Delay(volatile uint32_t nCount)
{
  while(nCount--)
  {
  }
}

void init_usart() {

    GPIO_InitTypeDef PORT;  //РЎС‚СЂСѓРєС‚СѓСЂР° СЃРѕРґРµСЂР¶Р°С‰Р°СЏ РЅР°СЃС‚СЂРѕР№РєРё РїРѕСЂС‚Р°
  USART_InitTypeDef USART;  //РЎС‚СЂСѓРєС‚СѓСЂР° СЃРѕРґРµСЂР¶Р°С‰Р°СЏ РЅР°СЃС‚СЂРѕР№РєРё USART

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //Р’РєР»СЋС‡Р°РµРј С‚Р°РєС‚РёСЂРѕРІР°РЅРёРµ РїРѕСЂС‚Р° USART1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //Р’РєР»СЋС‡Р°РµРј С‚Р°РєС‚РёСЂРѕРІР°РЅРёРµ РїРѕСЂС‚Р° A

    //РџРёРЅС‹ PA9 Рё PA10 РІ СЂРµР¶РёРјРµ Р°Р»СЊС‚РµСЂРЅР°С‚РёРІРЅС‹С… С„СѓРЅРєС†РёР№ вЂ“ Rx Рё Tx USARTвЂ™Р°
    GPIO_StructInit(&PORT);
    // Р·Р°РґР°РµРј РїР°СЂР°РјРµС‚СЂС‹ РІС‹РІРѕРґРѕРІ
    PORT.GPIO_Pin = GPIO_Pin_9;
    PORT.GPIO_Mode = GPIO_Mode_AF_PP;
    PORT.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA , &PORT);

    PORT.GPIO_Pin = GPIO_Pin_10;
    PORT.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    PORT.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA , &PORT);

    //РќР°СЃС‚СЂРѕР№РєР° USART
    USART_StructInit(&USART);
    USART.USART_BaudRate = 9600;   //РЎРєРѕСЂРѕСЃС‚СЊ РѕР±РјРµРЅР° 9600 Р±РѕРґ
    USART.USART_WordLength = USART_WordLength_8b; //Р”Р»РёРЅР° СЃР»РѕРІР° 8 Р±РёС‚
    USART.USART_StopBits = USART_StopBits_1; //1 СЃС‚РѕРї-Р±РёС‚
    USART.USART_Parity = USART_Parity_No ; //Р‘РµР· РїСЂРѕРІРµСЂРєРё С‡РµС‚РЅРѕСЃС‚Рё
    USART.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Р‘РµР· Р°РїРїР°СЂР°С‚РЅРѕРіРѕ РєРѕРЅС‚СЂРѕР»СЏ
    USART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Р’РєР»СЋС‡РµРЅ РїРµСЂРµРґР°С‚С‡РёРє Рё РїСЂРёРµРјРЅРёРє USART2
    USART_Init(USART1, &USART);

    USART_Cmd(USART1, ENABLE);  //Р’РєР»СЋС‡Р°РµРј UART

    //Р’РєР»СЋС‡Р°РµРј РїСЂРµСЂС‹РІР°РЅРёСЏ РїРѕ РїСЂРёРµРјСѓ Р±Р°Р№С‚Р° Рё РїРѕ РѕРєРѕРЅС‡Р°РЅРёСЋ РїРµСЂРµРґР°С‡Рё
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    //USART_ITConfig(USART1, USART_IT_TC, ENABLE);

}

void send(char chr) {
    while(!(USART1->SR & USART_SR_TC));
    USART1->DR = chr;
}

void send_str(char* str) {
    int i=0;
    while(str[i])
        send(str[i++]);
}

static __IO uint32_t TimingDelay = 0;
uint8_t usartData;
char buf[16];

void SysTick_Handler(void);
void Delay_US(__IO uint32_t nTime);

int main(void)
{



  SysTick_Config(SystemCoreClock / 1000);
  SystemInit(); // Р�РЅРёС†РёР°Р»РёР·Р°С†РёСЏ РЅР°СЃС‚СЂРѕРµРє РєРІР°СЂС†Р°

  __enable_irq();
  init_usart();
  NVIC_EnableIRQ(USART1_IRQn);

  send_str("UART OK");



  UB_Led_Init(); // Р�РЅРёС†РёР°Р»РёР·Р°С†РёСЏ СЃРІРµС‚РѕРґРёРѕРґРѕРІ

  ST7735_Init();
  ST7735_AddrSet(0,0,159,127);
  ST7735_Clear(COLOR565_BLACK);
  ST7735_Orientation(scr_CCW);
  ST7735_PutStr5x7(1, 0, 0, "   TEST BOARD STM32F103  ", COLOR565_DARK_ORANGE   , COLOR565_BLACK);
  ST7735_HLine(0,159,8,COLOR565_DARK_ORANGE);

  UB_Led_On(LED_DEBUG); // Р’РєР»СЋС‡РµРЅРёРµ Р·РµР»РµРЅРѕРіРѕ СЃРІРµС‚РѕРґРёРѕРґР°



  while(1)
  {
    Delay_US(500);
    UB_Led_Toggle(LED_DEBUG); // РњРёРіР°РЅРёРµ РєСЂР°СЃРЅС‹Рј СЃРІРµС‚РѕРґРёРѕРґРѕРј

  }
}


void Delay_US(__IO uint32_t nTime)
{
  TimingDelay = nTime;
  while(TimingDelay != 0);
}

void SysTick_Handler()
{
 if (TimingDelay != 0) TimingDelay--;
}

/*******************************************************************/
void USART1_IRQHandler()
{
    //РџСЂРѕРІРµСЂСЏРµРј, РґРµР№СЃС‚РІРёС‚РµР»СЊРЅРѕ Р»Рё РїСЂРµСЂС‹РІР°РЅРёРµ РІС‹Р·РІР°РЅРѕ РїСЂРёРµРјРѕРј РЅРѕРІРѕРіРѕ Р±Р°Р№С‚Р°
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
	usartData = USART_ReceiveData(USART1);
	//send_str("RECIEVE DATA");
	sprintf(buf, "%c",  usartData);
	ST7735_PutStr5x7(2, 0, 16, buf , COLOR565_YELLOW_GREEN, COLOR565_BLACK);
	usartData = 0;

    }
}

/****************************End of file****************************/

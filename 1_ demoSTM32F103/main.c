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

    GPIO_InitTypeDef PORT;  //Структура содержащая настройки порта
  USART_InitTypeDef USART;  //Структура содержащая настройки USART

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //Включаем тактирование порта USART1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //Включаем тактирование порта A

    //Пины PA9 и PA10 в режиме альтернативных функций – Rx и Tx USART’а
    GPIO_StructInit(&PORT);
    // задаем параметры выводов
    PORT.GPIO_Pin = GPIO_Pin_9;
    PORT.GPIO_Mode = GPIO_Mode_AF_PP;
    PORT.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA , &PORT);

    PORT.GPIO_Pin = GPIO_Pin_10;
    PORT.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    PORT.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA , &PORT);

    //Настройка USART
    USART_StructInit(&USART);
    USART.USART_BaudRate = 9600;   //Скорость обмена 9600 бод
    USART.USART_WordLength = USART_WordLength_8b; //Длина слова 8 бит
    USART.USART_StopBits = USART_StopBits_1; //1 стоп-бит
    USART.USART_Parity = USART_Parity_No ; //Без проверки четности
    USART.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
    USART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Включен передатчик и приемник USART2
    USART_Init(USART1, &USART);

    USART_Cmd(USART1, ENABLE);  //Включаем UART

    //Включаем прерывания по приему байта и по окончанию передачи
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
  SystemInit(); // Инициализация настроек кварца

  __enable_irq();
  init_usart();
  NVIC_EnableIRQ(USART1_IRQn);

  send_str("UART OK");



  UB_Led_Init(); // Инициализация светодиодов

  ST7735_Init();
  ST7735_AddrSet(0,0,159,127);
  ST7735_Clear(COLOR565_BLACK);
  ST7735_Orientation(scr_CCW);
  ST7735_PutStr5x7(1, 0, 0, "   TEST BOARD STM32F103  ", COLOR565_DARK_ORANGE   , COLOR565_BLACK);
  ST7735_HLine(0,159,8,COLOR565_DARK_ORANGE);

  UB_Led_On(LED_DEBUG); // Включение зеленого светодиода



  while(1)
  {
    Delay_US(500);
    UB_Led_Toggle(LED_DEBUG); // Мигание красным светодиодом

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
    //Проверяем, действительно ли прерывание вызвано приемом нового байта
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
	usartData = USART_ReceiveData(USART1);
	send_str("RECIEVE DATA");
	sprintf(buf, "%c",  usartData);
	ST7735_PutStr5x7(2, 0, 16, buf , COLOR565_YELLOW_GREEN, COLOR565_BLACK);
	usartData = 0;

    }
}

/****************************End of file****************************/

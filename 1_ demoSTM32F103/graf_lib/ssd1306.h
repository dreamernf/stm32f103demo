//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Ёто программное обеспечение распростран€етс€ свободно. ¬ы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Ёлектроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// јвтор: Ќадыршин –услан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _SSD1306_H
#define _SSD1306_H

#include <types.h>

#define SSD1306_IfMode_Par8080                  0       // ѕараллельный интерфейс 8080 8-бит
#define SSD1306_IfMode_Par6800                  1       // ѕараллельный интерфейс 6800 8-бит
#define SSD1306_IfMode_SPI_3w                   2       // SPI по 3 лини€м (CS, SCLK, MOSI)
#define SSD1306_IfMode_SPI_4w                   3       // SPI по 4 лини€м (CS, SCLK, MOSI, D/C)
#define SSD1306_IfMode_I2C                      4       // I2C

// »нтерфейс подключени€ диспле€      
#define SSD1306_IfMode          SSD1306_IfMode_SPI_4w //SSD1306_IfMode_I2C


// –ежимы автоматического сдвига указател€ в пам€ти кадра ssd1306
#define SSD1306_Adressing_Horizontal            0       // —начала инкремент по горизонтали, затем инкремент по вертикали
#define SSD1306_Adressing_Vertical              1       // —начала инкремент по вертикали, затем инкремент по горизонтали
#define SSD1306_Adressing_Page                  2       // »нкремент только по горизонтали


//==============================================================================
//  оды команд диспле€
//==============================================================================
//  оманды из раздела Fundamental
#define SSD1306_CMD_SetContrast                 0x81
#define SSD1306_CMD_AllPixRAM                   0xA4
#define SSD1306_CMD_AllPixOn                    0xA5
#define SSD1306_CMD_SetInverseOff               0xA6
#define SSD1306_CMD_SetInverseOn                0xA7
#define SSD1306_CMD_Sleep                       0xAE
#define SSD1306_CMD_Wake                        0xAF

//  оманды из раздела Scrolling
#define SSD1306_CMD_DeactivateScroll            0x2E

//  оманды из раздела Addressing Setting
//  оманда выбора режима автосдвига указател€ в пам€ти кадра
#define SSD1306_CMD_SetMemAdressingMode         0x20    
//  оманды выбора диапазона изменени€ страницы и колонки при автосдвиге указател€ в пам€ти кадра
// ѕримен€етс€ дл€ режимов автосдвига SSD1306_Adressing_Horizontal и SSD1306_Adressing_Vertical
#define SSD1306_CMD_SetColumnAddr               0x21
#define SSD1306_CMD_SetPageAddr                 0x22
//  оманды выбора страницы и диапазона изменени€ колонки при автосдвиге указател€ в пам€ти кадра
// ѕримен€етс€ дл€ режима автосдвига SSD1306_Adressing_Page
#define SSD1306_CMD_PageAddrMode_SetPage        0xB0
#define SSD1306_CMD_PageAddrMode_StartColumnLo  0x00
#define SSD1306_CMD_PageAddrMode_StartColumnHi  0x10

//  оманды из раздела Hardware Configuration
#define SSD1306_CMD_SetDisplayStartLine         0x40
#define SSD1306_CMD_SetSegmentRemap             0xA0
#define SSD1306_CMD_SetMultiplexRatio           0xA8
#define SSD1306_CMD_SetCOMoutScanDirection      0xC0 
#define SSD1306_CMD_SetDisplayOffset            0xD3
#define SSD1306_CMD_SetCOMPinsConfig            0xDA
  
//  оманды из раздела Timing & Driving Scheme Setting
#define SSD1306_CMD_SetDisplayClockDivider      0xD5
#define SSD1306_CMD_SetPrechargePeriod          0xD9
#define SSD1306_CMD_SetVCOMHDeselectLevel       0xDB

//  оманды из раздела Charge Pump
#define SSD1306_CMD_ChargePumpSetting           0x8D
//==============================================================================


//==============================================================================
// Ќастройки подключени€ к дисплею
//==============================================================================
#define SSD1306_ResetPinUsed          1       // »спользуетс€ управление пином Reset ssd1306      
#define SSD1306_Reset_Port            GPIOA
#define SSD1306_Reset_Pin             GPIO_Pin_4

#if (SSD1306_IfMode == SSD1306_IfMode_I2C)       // ѕараметры работы по I2C
  // i2c-адрес контроллера диспле€. «ависит от состо€ни€ ножки SA0 (она же D/C). 0x3C или 0x3D
  #define SSD1306_I2C_addr              0x3C //0x3D;   // 7-битный i2c-адрес контроллера диспле€ ssd1306      
  #define SSD1306_I2C_periph            I2C1    // I2C1 или I2C2        
  #define SSD1306_I2C_clock             100000  // „астота работы I2C        
  #define SSD1306_I2C_timeout           1000 //1000    // “аймаут ожидани€ выполнени€ операций на i2c      
#elif (SSD1306_IfMode == SSD1306_IfMode_SPI_4w)       // ѕараметры работы по SPI-4w
  #define SSD1306_SPI_periph            SPI1    // SPI1 или SPI2        
  #define SSD1306_CSPinUsed             0       // »спользуетс€ управление пином CS ssd1306      
  #define SSD1306_CS_Port               GPIOA
  #define SSD1306_CS_Pin                GPIO_Pin_2
  #define SSD1306_DC_Port               GPIOA
  #define SSD1306_DC_Pin                GPIO_Pin_3
#elif (SSD1306_IfMode == SSD1306_IfMode_SPI_3w)       // ѕараметры работы по SPI-3w
  #error –абота по выбранному интерфейсу не поддерживаетс€. јппаратный SPI не работает в 9-битном режиме
#else
  #error –абота по выбранному интерфейсу ещЄ не реализована (см. константу SSD1306_IfMode)
#endif
//==============================================================================


// ѕроцедура инициализации диспле€ на контроллере ssd1306
void SSD1306_Init(uint8_t Width, uint8_t Height);
// ѕроцедура переводит дисплей в режим сна
void SSD1306_Sleep(void);
// ѕроцедура выводит дисплей из режима сна
void SSD1306_Wake(void);
// ѕроцедура включает инверсию диспле€
void SSD1306_SetInverseOn(void);
// ѕроцедура отключает инверсию диспле€
void SSD1306_SetInverseOff(void);
// ѕроцедура включает все пиксели диспле€ (“ест индикатора)
void SSD1306_AllPixOn(void);
// ѕроцедура отключает тест диспле€ и выводит на него картинку из буфера кадра в ssd1306
void SSD1306_AllPixRAM(void);
// ѕроцедура устанавливает параметр контрастности (0-255)
void SSD1306_SetContrast(uint8_t Value);
// ѕроцедура устанавливает начальный и конечный индекс колонки 
// дл€ автосмещени€ указател€ в пам€ти кадра при чтении записи.
void SSD1306_SetColumns(uint8_t Start, uint8_t End);
// ѕроцедура устанавливает начальный и конечный индекс страницы 
// дл€ автосмещени€ указател€ в пам€ти кадра при чтении записи.
void SSD1306_SetPages(uint8_t Start, uint8_t End);
// ѕроцедура передаЄт в дисплей буфер кадра из массива pBuff
void SSD1306_DisplayFullUpdate(uint8_t *pBuff, uint16_t BuffLen);

#endif
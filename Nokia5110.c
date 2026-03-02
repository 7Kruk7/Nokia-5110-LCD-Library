/**
 * @file Nokia5110.c
 * @brief Implementacja biblioteki wyświetlacza Nokia 5110 dla Kinetis KL05Z4
 * @author Adaptacja dla KL05Z4 na podstawie biblioteki Nitin Sharma (Arduino)
 * @version 1.0
 */

#include "Nokia5110.h"


static GPIO_Type *g_rst_port;
static uint32_t g_rst_mask;
static GPIO_Type *g_ce_port;
static uint32_t g_ce_mask;
static GPIO_Type *g_dc_port;
static uint32_t g_dc_mask;
static GPIO_Type *g_din_port;
static uint32_t g_din_mask;
static GPIO_Type *g_clk_port;
static uint32_t g_clk_mask;


static const uint8_t ASCII_FONT[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // 0x20 (spacja)
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // 0x21 !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // 0x22 "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // 0x23 #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // 0x24 $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // 0x25 %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // 0x26 &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // 0x27 '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // 0x28 (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // 0x29 )
    {0x14, 0x08, 0x3E, 0x08, 0x14}, // 0x2A *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // 0x2B +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // 0x2C ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // 0x2D -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // 0x2E .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // 0x2F /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0x30 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 0x31 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 0x32 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 0x33 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 0x34 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 0x35 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 0x36 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 0x37 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 0x38 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 0x39 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // 0x3A :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // 0x3B ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // 0x3C <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // 0x3D =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // 0x3E >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // 0x3F ?
    {0x32, 0x49, 0x79, 0x41, 0x3E}, // 0x40 @
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // 0x41 A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // 0x42 B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // 0x43 C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // 0x44 D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // 0x45 E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // 0x46 F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // 0x47 G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // 0x48 H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // 0x49 I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // 0x4A J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // 0x4B K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // 0x4C L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // 0x4D M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // 0x4E N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 0x4F O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // 0x50 P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // 0x51 Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // 0x52 R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // 0x53 S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // 0x54 T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // 0x55 U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // 0x56 V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // 0x57 W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // 0x58 X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // 0x59 Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // 0x5A Z
    {0x00, 0x7F, 0x41, 0x41, 0x00}, // 0x5B [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // 0x5C (backslash)
    {0x00, 0x41, 0x41, 0x7F, 0x00}, // 0x5D ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // 0x5E ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // 0x5F _
    {0x00, 0x01, 0x02, 0x04, 0x00}, // 0x60 `
    {0x20, 0x54, 0x54, 0x54, 0x78}, // 0x61 a
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // 0x62 b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // 0x63 c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // 0x64 d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // 0x65 e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // 0x66 f
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // 0x67 g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // 0x68 h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // 0x69 i
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // 0x6A j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // 0x6B k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // 0x6C l
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // 0x6D m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // 0x6E n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // 0x6F o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // 0x70 p
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // 0x71 q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // 0x72 r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // 0x73 s
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // 0x74 t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // 0x75 u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // 0x76 v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // 0x77 w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // 0x78 x
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // 0x79 y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // 0x7A z
    {0x00, 0x08, 0x36, 0x41, 0x00}, // 0x7B {
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // 0x7C |
    {0x00, 0x41, 0x36, 0x08, 0x00}, // 0x7D }
    {0x10, 0x08, 0x08, 0x10, 0x08}, // 0x7E ~
    {0x78, 0x46, 0x41, 0x46, 0x78}  // 0x7F (strzałka)
};

/**
 * @brief Ustawienie pinu GPIO na wysoki stan
 */
static inline void GPIO_SetPin(GPIO_Type *port, uint32_t mask) {
    port->PSOR = mask;
}

/**
 * @brief Ustawienie pinu GPIO na niski stan
 */
static inline void GPIO_ClearPin(GPIO_Type *port, uint32_t mask) {
    port->PCOR = mask;
}

/**
 * @brief Wysłanie pojedynczego bajtu przez interfejs SPI (bit-banging)
 * @param data Bajt do wysłania (MSB first)
 */
static void SPI_SendByte(uint8_t data) {
    for (int i = 7; i >= 0; i--) {
        // Ustaw dane
        if (data & (1 << i)) {
            GPIO_SetPin(g_din_port, g_din_mask);
        } else {
            GPIO_ClearPin(g_din_port, g_din_mask);
        }
        
        // Zbocze narastające CLK
        GPIO_SetPin(g_clk_port, g_clk_mask);
        
        // Krótkie opóźnienie
        __NOP(); __NOP(); __NOP(); __NOP();
        
        // Zbocze opadające CLK
        GPIO_ClearPin(g_clk_port, g_clk_mask);
    }
}

/**
 * @brief Wysłanie komendy do wyświetlacza
 * @param cmd Komenda
 */
static void LCD_WriteCommand(uint8_t cmd) {
    GPIO_ClearPin(g_dc_port, g_dc_mask);   // DC = 0 (komenda)
    GPIO_ClearPin(g_ce_port, g_ce_mask);   // CE = 0 (aktywny)
    SPI_SendByte(cmd);
    GPIO_SetPin(g_ce_port, g_ce_mask);     // CE = 1 (nieaktywny)
}


void Nokia5110_DelayUs(uint32_t us) {
    // Przybliżone opóźnienie dla zegara ~41.94 MHz
    // Każda iteracja to około 6-10 cykli
    volatile uint32_t count = us * 7;
    while (count--) {
        __NOP();
    }
}

void Nokia5110_DelayMs(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        Nokia5110_DelayUs(1000);
    }
}


void Nokia5110_Init(void) {
    // Domyślna konfiguracja pinów
    Nokia5110_PinConfig config = {
        .rst_port = PTB,  .rst_pin = LCD_RST_PIN,
        .ce_port = PTB,   .ce_pin = LCD_CE_PIN,
        .dc_port = PTB,   .dc_pin = LCD_DC_PIN,
        .din_port = PTB,  .din_pin = LCD_DIN_PIN,
        .clk_port = PTA,  .clk_pin = LCD_CLK_PIN
    };
    
    Nokia5110_InitCustom(&config);
}

void Nokia5110_InitCustom(Nokia5110_PinConfig *config) {
    // Zapisz konfigurację globalnie
    g_rst_port = config->rst_port;
    g_rst_mask = (1UL << config->rst_pin);
    g_ce_port = config->ce_port;
    g_ce_mask = (1UL << config->ce_pin);
    g_dc_port = config->dc_port;
    g_dc_mask = (1UL << config->dc_pin);
    g_din_port = config->din_port;
    g_din_mask = (1UL << config->din_pin);
    g_clk_port = config->clk_port;
    g_clk_mask = (1UL << config->clk_pin);
    
    // Włącz zegary dla portów
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;
    
    // Konfiguracja pinów jako GPIO
    // RST
    if (config->rst_port == PTA) {
        PORTA->PCR[config->rst_pin] = PORT_PCR_MUX(1);
    } else {
        PORTB->PCR[config->rst_pin] = PORT_PCR_MUX(1);
    }
    
    // CE
    if (config->ce_port == PTA) {
        PORTA->PCR[config->ce_pin] = PORT_PCR_MUX(1);
    } else {
        PORTB->PCR[config->ce_pin] = PORT_PCR_MUX(1);
    }
    
    // DC
    if (config->dc_port == PTA) {
        PORTA->PCR[config->dc_pin] = PORT_PCR_MUX(1);
    } else {
        PORTB->PCR[config->dc_pin] = PORT_PCR_MUX(1);
    }
    
    // DIN
    if (config->din_port == PTA) {
        PORTA->PCR[config->din_pin] = PORT_PCR_MUX(1);
    } else {
        PORTB->PCR[config->din_pin] = PORT_PCR_MUX(1);
    }
    
    // CLK
    if (config->clk_port == PTA) {
        PORTA->PCR[config->clk_pin] = PORT_PCR_MUX(1);
    } else {
        PORTB->PCR[config->clk_pin] = PORT_PCR_MUX(1);
    }
    
    // Konfiguracja kierunku pinów jako wyjścia
    g_rst_port->PDDR |= g_rst_mask;
    g_ce_port->PDDR |= g_ce_mask;
    g_dc_port->PDDR |= g_dc_mask;
    g_din_port->PDDR |= g_din_mask;
    g_clk_port->PDDR |= g_clk_mask;
    
    // Stany początkowe
    GPIO_SetPin(g_ce_port, g_ce_mask);     // CE = 1 (nieaktywny)
    GPIO_ClearPin(g_clk_port, g_clk_mask); // CLK = 0
    GPIO_SetPin(g_rst_port, g_rst_mask);   // RST = 1 (inactive)
    
    // delay bo bez tego gowno dziala
    Nokia5110_DelayMs(100);
    
    GPIO_ClearPin(g_rst_port, g_rst_mask); // RST = 0 (active)
    Nokia5110_DelayMs(50);           
    GPIO_SetPin(g_rst_port, g_rst_mask);   // RST = 1 (inactive)
    Nokia5110_DelayMs(50);
    
    // Inicjalizacja sterownika PCD8544
    LCD_WriteCommand(PCD8544_FUNCTIONSET | PCD8544_EXTINSTRUCTION); // Tryb rozszerzony
    Nokia5110_DelayMs(1);
    LCD_WriteCommand(PCD8544_SETVOP | 0x40);    // Kontrast (0x40 = 64, lekko wyższy)
    Nokia5110_DelayMs(1);
    LCD_WriteCommand(PCD8544_SETTEMP | 0x02);   // Współczynnik temperaturowy = 2
    Nokia5110_DelayMs(1);
    LCD_WriteCommand(PCD8544_SETBIAS | 0x03);   // Bias 1:48
    Nokia5110_DelayMs(1);
    LCD_WriteCommand(PCD8544_FUNCTIONSET);      // Tryb podstawowy
    Nokia5110_DelayMs(1);
    LCD_WriteCommand(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL); // Normalne wyświetlanie
    Nokia5110_DelayMs(1);
    
    // Wyczyść ekran
    Nokia5110_Clear();
}

void Nokia5110_WriteData(uint8_t data) {
    GPIO_SetPin(g_dc_port, g_dc_mask);     // DC = 1 (dane)
    GPIO_ClearPin(g_ce_port, g_ce_mask);   // CE = 0 (aktywny)
    SPI_SendByte(data);
    GPIO_SetPin(g_ce_port, g_ce_mask);     // CE = 1 (nieaktywny)
}

void Nokia5110_Clear(void) {
    Nokia5110_SetXY(0, 0);
    for (int i = 0; i < LCD_BUFFER_SIZE; i++) {
        Nokia5110_WriteData(0x00);
    }
    Nokia5110_SetXY(0, 0);
}

void Nokia5110_SetXY(uint8_t x, uint8_t y) {
    LCD_WriteCommand(PCD8544_SETXADDR | (x & 0x7F));  // Kolumna (0-83)
    LCD_WriteCommand(PCD8544_SETYADDR | (y & 0x07));  // Wiersz (0-5)
}


void Nokia5110_WriteChar(char c) {
    // Sprawdź zakres znaków
    if (c < 0x20 || c > 0x7F) {
        c = 0x20; // Zamień nieobsługiwane znaki na spację
    }
    
    // Wyślij 5 kolumn znaku
    for (int i = 0; i < 5; i++) {
        Nokia5110_WriteData(ASCII_FONT[c - 0x20][i]);
    }
    
    // Wyślij odstęp między znakami (1 piksel)
    Nokia5110_WriteData(0x00);
}

void Nokia5110_WriteString(const char *str) {
    while (*str) {
        Nokia5110_WriteChar(*str++);
    }
}

void Nokia5110_WriteStringXY(uint8_t x, uint8_t y, const char *str) {
    Nokia5110_SetXY(x, y);
    Nokia5110_WriteString(str);
}

void Nokia5110_WriteUInt(uint32_t num) {
    char buffer[12];
    int i = 0;
    
    if (num == 0) {
        Nokia5110_WriteChar('0');
        return;
    }
    
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    while (i > 0) {
        Nokia5110_WriteChar(buffer[--i]);
    }
}


void Nokia5110_SetContrast(uint8_t value) {
    if (value > 127) value = 127;
    
    LCD_WriteCommand(PCD8544_FUNCTIONSET | PCD8544_EXTINSTRUCTION);
    LCD_WriteCommand(PCD8544_SETVOP | value);
    LCD_WriteCommand(PCD8544_FUNCTIONSET);
}
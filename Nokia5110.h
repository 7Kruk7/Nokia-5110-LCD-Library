/**
 * @file Nokia5110.h
 * @brief Nokia 5110 display library for Kinetis KL05Z4 microcontroller
 * @author Adaptation for KL05Z4 based on Nitin Sharma's library (Arduino)
 * @version 1.0
 * 
 * Nokia 5110 display (PCD8544 driver):
 * - Resolution: 84x48 pixels
 * - Interface: SPI-like (bit-banging)
 * - Supply voltage: 2.7V - 3.3V
 * 
 * Default pin connections (can be changed in Nokia5110_Init):
 *   RST  -> PTB8
 *   CE   -> PTB9
 *   DC   -> PTB10
 *   DIN  -> PTB11
 *   CLK  -> PTA12
 *   VCC  -> 3.3V
 *   BL   -> 3.3V (backlight)
 *   GND  -> GND
 */

#ifndef NOKIA5110_H
#define NOKIA5110_H

#include "MKL05Z4.h"
#include <stdint.h>

// ============================================================================
// Pin definitions - DEFAULT CONFIGURATION (Port B + PTA12)
// ============================================================================
// Any GPIO pins can be used; the following are examples

// Port B pins
#define LCD_RST_PIN     8       // PTB8 - Reset
#define LCD_CE_PIN      9       // PTB9 - Chip Enable (active low)
#define LCD_DC_PIN      10      // PTB10 - Data/Command (0=cmd, 1=data)
#define LCD_DIN_PIN     11      // PTB11 - Data In (MOSI)

// Port A pin
#define LCD_CLK_PIN     12      // PTA12 - Clock

// Bit masks for faster access
#define LCD_RST_MASK    (1UL << LCD_RST_PIN)
#define LCD_CE_MASK     (1UL << LCD_CE_PIN)
#define LCD_DC_MASK     (1UL << LCD_DC_PIN)
#define LCD_DIN_MASK    (1UL << LCD_DIN_PIN)
#define LCD_CLK_MASK    (1UL << LCD_CLK_PIN)


// Nokia 5110 display parameters
#define LCD_WIDTH       84      // Width in pixels
#define LCD_HEIGHT      48      // Height in pixels
#define LCD_ROWS        6       // Number of text rows (48/8)
#define LCD_COLS        14      // Number of character columns (84/6)
#define LCD_BUFFER_SIZE 504     // Buffer size (84 * 48 / 8)

// PCD8544 driver commands
#define PCD8544_FUNCTIONSET     0x20    // Function set
#define PCD8544_DISPLAYCONTROL  0x08    // Display control
#define PCD8544_SETYADDR        0x40    // Set Y address of RAM
#define PCD8544_SETXADDR        0x80    // Set X address of RAM

// Function set flags
#define PCD8544_ENTRYMODE       0x02    // Entry mode
#define PCD8544_EXTINSTRUCTION  0x01    // Extended instruction set

// Display control flags
#define PCD8544_DISPLAYBLANK    0x00    // Display blank
#define PCD8544_DISPLAYNORMAL   0x04    // Normal mode
#define PCD8544_DISPLAYALLON    0x01    // All segments on
#define PCD8544_DISPLAYINVERTED 0x05    // Inverse mode

// Extended instruction set
#define PCD8544_SETVOP          0x80    // Set Vop (contrast)
#define PCD8544_SETTEMP         0x04    // Set temperature coefficient
#define PCD8544_SETBIAS         0x10    // Set bias system

typedef struct {
    GPIO_Type *rst_port;    // Port for RST (e.g. PTB)
    uint32_t rst_pin;       // RST pin number
    GPIO_Type *ce_port;     // Port for CE
    uint32_t ce_pin;        // CE pin number
    GPIO_Type *dc_port;     // Port for DC
    uint32_t dc_pin;        // DC pin number
    GPIO_Type *din_port;    // Port for DIN (data)
    uint32_t din_pin;       // DIN pin number
    GPIO_Type *clk_port;    // Port for CLK
    uint32_t clk_pin;       // CLK pin number
} Nokia5110_PinConfig;


/**
 * @brief Initialize display with default pin configuration
 * 
 * Uses default pins:
 *   RST=PTB8, CE=PTB9, DC=PTB10, DIN=PTB11, CLK=PTA12
 */
void Nokia5110_Init(void);

/**
 * @brief Initialize display with custom pin configuration
 * @param config Pointer to pin configuration structure
 */
void Nokia5110_InitCustom(Nokia5110_PinConfig *config);

/**
 * @brief Clear entire display
 */
void Nokia5110_Clear(void);

/**
 * @brief Set cursor position
 * @param x Column (0-83)
 * @param y Row (0-5)
 */
void Nokia5110_SetXY(uint8_t x, uint8_t y);

/**
 * @brief Display single character
 * @param c ASCII character to display
 */
void Nokia5110_WriteChar(char c);

/**
 * @brief Display string
 * @param str Pointer to null-terminated string
 */
void Nokia5110_WriteString(const char *str);

/**
 * @brief Display string at specified position
 * @param x Column (0-83)
 * @param y Row (0-5)
 * @param str Pointer to string
 */
void Nokia5110_WriteStringXY(uint8_t x, uint8_t y, const char *str);

/**
 * @brief Set display contrast
 * @param value Contrast value (0-127), typically 40-60
 */
void Nokia5110_SetContrast(uint8_t value);

/**
 * @brief Send raw data to display
 * @param data Data byte
 */
void Nokia5110_WriteData(uint8_t data);

void Nokia5110_WriteUInt(uint32_t num);

/**
 * @brief Delay in microseconds (approximate)
 * @param us Number of microseconds
 */
void Nokia5110_DelayUs(uint32_t us);

/**
 * @brief Delay in milliseconds
 * @param ms Number of milliseconds
 */
void Nokia5110_DelayMs(uint32_t ms);

#endif // NOKIA5110_H

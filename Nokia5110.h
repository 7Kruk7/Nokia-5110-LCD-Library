/**
 * @file Nokia5110.h
 * @brief Biblioteka wyświetlacza Nokia 5110 dla mikrokontrolera Kinetis KL05Z4
 * @author Adaptacja dla KL05Z4 na podstawie biblioteki Nitin Sharma (Arduino)
 * @version 1.0
 * 
 * Wyświetlacz Nokia 5110 (sterownik PCD8544):
 * - Rozdzielczość: 84x48 pikseli
 * - Interfejs: SPI-podobny (bit-banging)
 * - Napięcie zasilania: 2.7V - 3.3V
 * 
 * Domyślne połączenia pinów (można zmienić w Nokia5110_Init):
 *   RST  -> PTB8
 *   CE   -> PTB9
 *   DC   -> PTB10
 *   DIN  -> PTB11
 *   CLK  -> PTA12
 *   VCC  -> 3.3V
 *   BL   -> 3.3V (podświetlenie)
 *   GND  -> GND
 */

#ifndef NOKIA5110_H
#define NOKIA5110_H

#include "MKL05Z4.h"
#include <stdint.h>

// ============================================================================
// Definicje pinów - DOMYŚLNA KONFIGURACJA (Port B + PTA12)
// ============================================================================
// Można użyć dowolnych pinów GPIO, poniższe są przykładowe

// Port B piny
#define LCD_RST_PIN     8       // PTB8 - Reset
#define LCD_CE_PIN      9       // PTB9 - Chip Enable (aktywny niski)
#define LCD_DC_PIN      10      // PTB10 - Data/Command (0=cmd, 1=data)
#define LCD_DIN_PIN     11      // PTB11 - Data In (MOSI)

// Port A pin
#define LCD_CLK_PIN     12      // PTA12 - Clock

// Maski bitowe dla szybszego dostępu
#define LCD_RST_MASK    (1UL << LCD_RST_PIN)
#define LCD_CE_MASK     (1UL << LCD_CE_PIN)
#define LCD_DC_MASK     (1UL << LCD_DC_PIN)
#define LCD_DIN_MASK    (1UL << LCD_DIN_PIN)
#define LCD_CLK_MASK    (1UL << LCD_CLK_PIN)


// Parametry wyświetlacza Nokia 5110
#define LCD_WIDTH       84      // Szerokość w pikselach
#define LCD_HEIGHT      48      // Wysokość w pikselach
#define LCD_ROWS        6       // Liczba wierszy tekstowych (48/8)
#define LCD_COLS        14      // Liczba kolumn znaków (84/6)
#define LCD_BUFFER_SIZE 504     // Rozmiar bufora (84 * 48 / 8)

// Komendy sterownika PCD8544
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
    GPIO_Type *rst_port;    // Port dla RST (np. PTB)
    uint32_t rst_pin;       // Numer pinu RST
    GPIO_Type *ce_port;     // Port dla CE
    uint32_t ce_pin;        // Numer pinu CE
    GPIO_Type *dc_port;     // Port dla DC
    uint32_t dc_pin;        // Numer pinu DC
    GPIO_Type *din_port;    // Port dla DIN (dane)
    uint32_t din_pin;       // Numer pinu DIN
    GPIO_Type *clk_port;    // Port dla CLK
    uint32_t clk_pin;       // Numer pinu CLK
} Nokia5110_PinConfig;


/**
 * @brief Inicjalizacja wyświetlacza z domyślną konfiguracją pinów
 * 
 * Używa domyślnych pinów:
 *   RST=PTB8, CE=PTB9, DC=PTB10, DIN=PTB11, CLK=PTA12
 */
void Nokia5110_Init(void);

/**
 * @brief Inicjalizacja wyświetlacza z własną konfiguracją pinów
 * @param config Wskaźnik na strukturę konfiguracji pinów
 */
void Nokia5110_InitCustom(Nokia5110_PinConfig *config);

/**
 * @brief Czyszczenie całego wyświetlacza
 */
void Nokia5110_Clear(void);

/**
 * @brief Ustawienie pozycji kursora
 * @param x Kolumna (0-83)
 * @param y Wiersz (0-5)
 */
void Nokia5110_SetXY(uint8_t x, uint8_t y);

/**
 * @brief Wyświetlenie pojedynczego znaku
 * @param c Znak ASCII do wyświetlenia
 */
void Nokia5110_WriteChar(char c);

/**
 * @brief Wyświetlenie ciągu znaków
 * @param str Wskaźnik na ciąg znaków (zakończony NULL)
 */
void Nokia5110_WriteString(const char *str);

/**
 * @brief Wyświetlenie ciągu znaków w określonej pozycji
 * @param x Kolumna (0-83)
 * @param y Wiersz (0-5)
 * @param str Wskaźnik na ciąg znaków
 */
void Nokia5110_WriteStringXY(uint8_t x, uint8_t y, const char *str);

/**
 * @brief Ustawienie kontrastu wyświetlacza
 * @param value Wartość kontrastu (0-127), typowo 40-60
 */
void Nokia5110_SetContrast(uint8_t value);

/**
 * @brief Wysłanie surowych danych do wyświetlacza
 * @param data Bajt danych
 */
void Nokia5110_WriteData(uint8_t data);

void Nokia5110_WriteUInt(uint32_t num);

/**
 * @brief Opóźnienie w mikrosekundach (przybliżone)
 * @param us Liczba mikrosekund
 */
void Nokia5110_DelayUs(uint32_t us);

/**
 * @brief Opóźnienie w milisekundach
 * @param ms Liczba milisekund
 */
void Nokia5110_DelayMs(uint32_t ms);

#endif // NOKIA5110_H

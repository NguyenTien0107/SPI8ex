#include <stdint.h>
#define F_CPU 2000000UL
#define _SFR_(mem_addr)     (*(volatile uint8_t *)(0x5000 + (mem_addr)))
/* SPI */
#define SPI_CR1     _SFR_(0x200)
#define SPE         6
#define BR0         3
#define MSTR        2
#define SPI_CR2     _SFR_(0x201)
#define SSM         1
#define SSI         0
#define SPI_SR      _SFR_(0x203)
#define BSY         7
#define TXE         1
#define RXNE        0
#define SPI_DR      _SFR_(0x204)
/* Chip select */
#define CS_PIN      4


/*
 * SPI pinout:
 * SCK  -> PC5
 * MOSI -> PC6
 * MISO -> PC7
 * CS   -> PC4
 */
void SPI_init() {
    /* Initialize CS pin */
    PC_DDR |= (1 << CS_PIN);
    PC_CR1 |= (1 << CS_PIN);
    PC_ODR |= (1 << CS_PIN);
    /* Initialize SPI master at 500kHz  */
    SPI_CR2 = (1 << SSM) | (1 << SSI);
    SPI_CR1 = (1 << MSTR) | (1 << SPE) | (1 << BR0);
}
void SPI_write(uint8_t data) {
    SPI_DR = data;
    while (!(SPI_SR & (1 << TXE)));
}
uint8_t SPI_read() {
    SPI_write(0xFF);
    while (!(SPI_SR & (1 << RXNE)));
    return SPI_DR;
}
void chip_select() {
    PC_ODR &= ~(1 << CS_PIN);
}
void chip_deselect() {
    PC_ODR |= (1 << CS_PIN);
}
void main() {
    SPI_init();
    while (1) {
        chip_select();
        for (uint8_t i = 0xAA; i < 0xFA; i += 0x10)
            SPI_write(i);
        chip_deselect();
    }
}

#include <serial.hpp>


namespace USART {


void USART::putc(uint8_t c) {

    // Get all the registers
    volatile uint8_t * const data_ptr = reinterpret_cast<volatile uint8_t *>(this->base_address + UDRn_OFFSET);
    volatile uint8_t * const a_ptr = reinterpret_cast<volatile uint8_t *>(this->base_address + UCSRnA_OFFSET);

    // Wait for the transmit buffer to have space for new data
    do {} while((*a_ptr & (1 << 5)) == 0);

    // Write the data and return
    *data_ptr = c;
}

void USART::putn(const uint8_t *buf, size_t len) {
    for(size_t i = 0; i < len; i++, buf++) {
        this->putc(*buf);
    }
}

void USART::puts(const char *buf) {
    for(; *buf != 0; buf++) {
        this->putc(*buf);
    }
}


}; // namespace USART

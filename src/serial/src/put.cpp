#include <serial.hpp>
#include <registers.hpp>


namespace USART {


void USART::putc(uint8_t c) {

    // Wait for the transmit buffer to have space for new data
    while((A_REG & (1 << 5)) == 0){
        // Spinlock
    }

    // Write the data and return
    DATA_REG = c;
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

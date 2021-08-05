#include <serial.hpp>


namespace USART {


Error USART::putc(uint8_t c) {

    // Get all the registers
    volatile uint8_t * const data_ptr = reinterpret_cast<volatile uint8_t *>(this->base_address + UDRn_OFFSET);
    volatile uint8_t * const a_ptr = reinterpret_cast<volatile uint8_t *>(this->base_address + UCSRnA_OFFSET);

    // Wait for the transmit buffer to have space for new data
    do {} while((*a_ptr & (1 << 5)) == 0);

    // Write the data and return
    *data_ptr = c;
    return Error::NONE;
}

Error USART::putn(const uint8_t *buf, size_t len) {

    // Collect all the errors
    Error ret = Error::NONE;

    // Iterate `len` times
    for(size_t i = 0; i < len; i++, buf++) {
        ret |= this->putc(*buf);
    }

    return ret;
}

Error USART::puts(const char *buf) {

    // Collect all the errors
    Error ret = Error::NONE;

    // Iterate until a null terminator
    for(; *buf != 0; buf++) {
        ret |= this->putc(*buf);
    }

    return ret;
}


}; // namespace USART

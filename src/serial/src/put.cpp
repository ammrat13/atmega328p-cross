#include <serial.hpp>
#include <registers.hpp>


namespace USART {


Error USART::putc(uint8_t c) {

    // Check to make sure the transmitter is enabled
    if(!this->configuration.tx_en) {
        return Error::DISABLED;
    }

    // Wait for the transmit buffer to have space for new data
    while((A_REG & (1 << 5)) == 0);

    // Write the data and return
    DATA_REG = c;
    return Error::NONE;
}

Error USART::putn(const uint8_t *buf, size_t len) {

    // Check to make sure the transmitter is enabled
    if(!this->configuration.tx_en) {
        return Error::DISABLED;
    }

    // Put `len` characters from `buf`
    for(size_t i = 0; i < len; i++, buf++) {
        this->putc(*buf);
    }

    return Error::NONE;
}

Error USART::puts(const char *buf) {

    // Check to make sure the transmitter is enabled
    if(!this->configuration.tx_en) {
        return Error::DISABLED;
    }

    // Put characters from `buf` until we hit a null terminator
    for(; *buf != 0; buf++) {
        this->putc(static_cast<uint8_t>(*buf));
    }

    return Error::NONE;
}


}; // namespace USART

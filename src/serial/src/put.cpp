#include <serial.hpp>
#include <registers.hpp>


namespace USART {


void USART::putcNoDisableCheck(uint8_t c) {

    // Wait for the transmit buffer to have space for new data
    // This is what blocks if the transmitter is disabled
    while((A_REG & (1 << 5)) == 0);

    // Write the data and return
    DATA_REG = c;
}


Error USART::putc(uint8_t c) {

    // Check to make sure the transmitter is enabled
    if(this->getConfiguration().tx_en == false) {
        return Error::DISABLED;
    }

    this->putcNoDisableCheck(c);
    return Error::NONE;
}

Error USART::putn(const uint8_t *buf, size_t len) {

    // Check to make sure the transmitter is enabled
    if(this->getConfiguration().tx_en == false) {
        return Error::DISABLED;
    }

    // Put `len` characters from `buf`
    for(size_t i = 0; i < len; i++, buf++) {
        this->putcNoDisableCheck(*buf);
    }

    return Error::NONE;
}

Error USART::puts(const char *buf) {

    // Check to make sure the transmitter is enabled
    if(this->getConfiguration().tx_en == false) {
        return Error::DISABLED;
    }

    // Put characters from `buf` until we hit a null terminator
    for(; *buf != 0; buf++) {
        this->putcNoDisableCheck(static_cast<uint8_t>(*buf));
    }

    return Error::NONE;
}


}; // namespace USART

#include <serial.hpp>


namespace USART {


Error USART::getc(uint8_t &c) {

    // Get all the registers
    volatile uint8_t * const data_ptr = reinterpret_cast<volatile uint8_t *>(this->base_address + UDRn_OFFSET);
    volatile uint8_t * const a_ptr = reinterpret_cast<volatile uint8_t *>(this->base_address + UCSRnA_OFFSET);

    // Wait for there to be data
    do {} while((*a_ptr & (1 << 7)) == 0);

    // Read the data and return
    // We have to read the return value first since it goes invalid when we read
    //  the data
    Error ret = this->getError();
    c = *data_ptr;
    return ret;
}

Error USART::getn(uint8_t *buf, size_t len, size_t &read) {

    // Keep track of any errors
    Error ret = Error::NONE;

    // Read the bytes
    // Keep track of whether we should continue. This way the increments still
    //  happen after the fact
    bool should_continue = true;
    for(read = 0; read < len && should_continue; read++, buf++) {
        // Read a character
        ret = this->getc(*buf);
        // Check if we should stop after this iteration
        if(ret != Error::NONE) {
            should_continue = false;
        }
    }

    return ret;
}

Error USART::gets(char *buf, size_t len, size_t &read, char until) {

    // Keep track of any errors
    Error ret = Error::NONE;

    // Read the bytes
    // Keep track of whether we should continue. This way the increments still
    //  happen after the fact
    bool should_continue = true;
    for(read = 0; read < len-1 && should_continue; read++, buf++) {
        // Read a character
        // Cast to a uint8_t pointer for types
        ret = this->getc(*reinterpret_cast<uint8_t*>(buf));
        // Check if we should stop after this iteration
        if(*buf == until || ret != Error::NONE) {
            should_continue = false;
        }
    }

    // Add the null terminator before returning
    *buf = 0;
    return ret;
}


}; // namespace USART

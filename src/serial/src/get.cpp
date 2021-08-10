#include <serial.hpp>
#include <registers.hpp>


namespace USART {


Error USART::getc(uint8_t &c) {

    // Check to make sure the receiver is enabled
    if(!this->configuration.rx_en) {
        return Error::DISABLED;
    }

    // Wait for there to be data in the recieve buffer
    while((A_REG & (1 << 7)) == 0);

    // Read the data and return
    // We have to read the return value first since it goes invalid when we read
    //  the data
    Error ret = this->getError();
    c = DATA_REG;
    return ret;
}

Error USART::getn(uint8_t *buf, size_t len, size_t &read) {

    // Check to make sure the receiver is enabled
    if(!this->configuration.rx_en) {
        return Error::DISABLED;
    }

    // Keep track of any errors
    Error ret = Error::NONE;

    // Read the bytes
    // Keep track of whether we should continue. This way the increments still
    //  happen after the fact
    bool should_continue = true;
    for(read = 0; read < len && should_continue; read++, buf++) {
        // Read a character
        ret = this->getc(*buf);
        // Check if we continue after this iteration
        should_continue = ret == Error::NONE;
    }

    return ret;
}

Error USART::gets(char *buf, size_t len, size_t &read, char until) {

    // Check to make sure the receiver is enabled
    if(!this->configuration.rx_en) {
        return Error::DISABLED;
    }

    // Keep track of any errors
    Error ret = Error::NONE;

    // Read the bytes
    // Keep track of whether we should continue. This way the increments still
    //  happen after the fact
    bool should_continue = true;
    for(read = 0; read < len-1 && should_continue; read++, buf++) {
        // Read a character
        // Cast to a uint8_t pointer for types
        ret = this->getc(*reinterpret_cast<uint8_t *>(buf));
        // Check if we should continue after this iteration
        should_continue = ret == Error::NONE && *buf != until;
    }

    // Add the null terminator before returning
    *buf = '\0';
    return ret;
}


}; // namespace USART
